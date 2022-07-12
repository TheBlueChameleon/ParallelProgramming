#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

#include <mpi.h>

#include "mpiHelpers.hpp"

const int debug = 0;

template <class T>
class TreeSendCarrier {
private:
    size_t dataLength;          // units of useful data; NOT size in bytes
    void*  data;                // both, header and useData

public:
    TreeSendCarrier(size_t first, size_t last, const std::vector<T>& vec) :
        dataLength( vec.size() )
    {
        data = new char[2 * sizeof(size_t) +                                    // space for header ...
                        vec.size() * sizeof(T)];                                // ... and useData

        setFirst(first);
        setLast (last );

        std::memcpy(data + 2 * sizeof(size_t),                                  // copy useData into buffer, after header
                    vec.data(),
                    dataLength * sizeof(T));
    }

    TreeSendCarrier(size_t msgLength) {                                         // prepare to receive message of msgLength bytes
        dataLength = (msgLength - 2 * sizeof(size_t)) / sizeof(T);
        data       = new char[msgLength];
    }

    ~TreeSendCarrier() {delete[] data;}

    size_t getFirst () const {return reinterpret_cast<size_t*>(data)[0];}
    size_t getLast  () const {return reinterpret_cast<size_t*>(data)[1];}
    T*     getData  () const {return reinterpret_cast<T*>(data + 2 * sizeof(size_t));}      // actual useData
    void * getBuffer() const {return data;}                                                 // buffer incl. header

    size_t getDataLength    () const {return                      dataLength            ;}  // units of data
    size_t getMessageLength () const {return 2 * sizeof(size_t) + dataLength * sizeof(T);}  // communication length incl. header

    void setFirst(size_t val) const {reinterpret_cast<size_t*>(data)[0] = val;}
    void setLast (size_t val) const {reinterpret_cast<size_t*>(data)[1] = val;}

    bool   isTerminal() const {return (getLast() - getFirst() == 0);}                       // opposite of "does this package need to be distributed further?"

    static constexpr int TAG = 99;
};

template <class T>
void spread(TreeSendCarrier<T>& tsc) {
    const size_t
        first = tsc.getFirst(),
        last  = tsc.getLast (),
        width = tsc.getLast() - tsc.getFirst(),
        mid   = tsc.getFirst() + (width >> 1);

    if (debug) {
        std::stringstream s;
        s << "process #" << procID << " spreads to:" << std::endl;
        s << "\t" << first << " -> [" << first << ", " << mid  << "]" << std::endl;
        s << "\t" << mid+1 << " -> [" << mid+1 << ", " << last << "]" << std::endl;

        showAndClear(s);
    }

    MPI_Request req_left,
                req_right;

    tsc.setLast (mid);
    MPI_Isend(tsc.getBuffer(), tsc.getMessageLength(), MPI_CHAR,
              first, tsc.TAG, MPI::COMM_WORLD, &req_left);

    tsc.setFirst(mid+1);
    tsc.setLast (last);
    MPI_Isend(tsc.getBuffer(), tsc.getMessageLength(), MPI_CHAR,
              mid+1, tsc.TAG, MPI::COMM_WORLD, &req_right);
}

std::vector<int> listenAndDistribute() {
    MPI_Status status;
    int msgSize;

    MPI_Probe(MPI_ANY_SOURCE, TreeSendCarrier<double>::TAG, MPI::COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_CHAR, &msgSize);

    TreeSendCarrier<int> tsc(msgSize);

    MPI_Recv(tsc.getBuffer(), msgSize, MPI_CHAR,
             MPI_ANY_SOURCE, TreeSendCarrier<double>::TAG, MPI::COMM_WORLD, &status);

    if (debug) {
        std::stringstream s;
        s << "process #" << procID << " got:" << std::endl;
        s << "\tfirst   : " << tsc.getFirst() << std::endl;
        s << "\tlast    : " << tsc.getLast () << std::endl;
        s << "\tterminal: " << (tsc.isTerminal() ? "yes" : "no") << std::endl;

        showAndClear(s);
    }

    if (!tsc.isTerminal()) { spread(tsc); }

    auto dataPtr = tsc.getData();
    auto N       = tsc.getDataLength();
    return std::vector<int>(dataPtr, dataPtr + N);
}

void task02_treeScatter() {
    const int root = 0;
    std::vector<int> data = {1,3,3,7}, received;

    if (procID == root) {
        auto tsc = TreeSendCarrier(0, Nprocs - 1, data);
        spread(tsc);
    }

    received = listenAndDistribute();

    std::stringstream s;
    s << "process #" << procID << " received:" << std::endl;
    for (const auto x : received) {
        s << "  " << x;
    }
    s << std::endl;
    showAndClear(s);
}
