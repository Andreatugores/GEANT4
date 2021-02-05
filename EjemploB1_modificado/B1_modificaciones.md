# Modificaciones Ejemplo B1

## B1DetectorConstruction
La configuración consiste en una envoltura con forma de caja que contiene dos volúmenes, una esfera cortada a la mitad y un elipsoide. Los materiales con los que está construido son agua para la envoltura y tejidos y material óseo para los volúmenes internos respectivamente.
     
Usando la base de datos NIST se cambió el material usado inicialmente en el segundo sólido, por G4\_BONE\_CORTICAL\_ICRP. Para poder construir el elipsoide se agregó: \#include "G4Ellipsoid.hh".
     
## B1PrimaryGeneratorAction
La cinemática predeterminada es un gamma de 6 MeV, distribuida aleatoriamente delante de la envolvente en el 80\% del plano transversal (X, Y). Se cambió la dirección del momento mediante los comandos de la clase ParticleGun.
     
