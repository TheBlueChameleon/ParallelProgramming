program inheritance
    implicit none

    ! ------------------------------------------------------------------------ !
    ! basic types

    type Motor_t
        real                :: Leistung
        real                :: Hubraum
        integer             :: Zylinder
        character(len=20)   :: Treibstoff
    end type

    type Reifen_t
        integer             :: Anzahl
        character(len=20)   :: Fabrikat
        real                :: Luftdruck
        real                :: Durchmesser
    end type

    type Fahrzeug_t
        character(len=20)   :: Fabrikat
        character(len=20)   :: Farbe
        character(len=20)   :: Kennzeichen
        integer             :: Sitzplaetze
        real                :: Gewicht
        type(Motor_t)       :: Motor
        type(Reifen_t)      :: Reifen
    end type

    ! ------------------------------------------------------------------------ !
    ! derived types

    type, extends(Fahrzeug_t) :: Auto_t
        real                :: Kofferraumvolumen
    end type

    type, extends(Fahrzeug_t) :: Bus_t
        integer             :: Stehplaetze
    end type

    type, extends(Fahrzeug_t) :: Lkw_t
        real                :: LadeVolumen
    end type

    ! ------------------------------------------------------------------------ !
    ! variables

    type(Motor_t)       :: motor
    type(Reifen_t)      :: reifen

    type(Fahrzeug_t)    :: fahrzeug

    type(Auto_t)        :: auto
    type(Bus_t)         :: bus
    type(Lkw_t)         :: lkw

    ! ------------------------------------------------------------------------ !
    ! assignment

    motor  = Motor_t (999, 0, -1, "Sugar Syrup")
    reifen = Reifen_t(4, "ACME Inc.", 200, 11)

    fahrzeug = Fahrzeug_t("foo", "bar", "FOO-BAR-000", 4, 999.8, motor, reifen)

    auto = Auto_t(fahrzeug, 666)
    bus  = Bus_t (fahrzeug, 666)
    lkw  = Lkw_t (fahrzeug, 666)

    print *, "FAHRZEUG:"
    print *, fahrzeug

    print *, "LKW:"
    print *, lkw
end program inheritance

