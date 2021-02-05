# Volumen esférico en una concha esférica

Se buscan obtener los valores de energía depositada y dosis total en el interior de un volumen esférico que se encuentra en el interior de una concha esférica. Se utiliza la lista física de GEANT4 QBBC, que se instancia en la función main(). El tipo de partículas y los procesos requieren archivos de datos para procesos electromagnéticos y hadrónicos. Los siguientes conjuntos de datos son obligatorios para este ejemplo: G4LEDATA, G4LEVELGAMMADATA, G4NEUTRONXSDATA, G4SAIDXSDATA y G4ENSDFSTATEDATA.

Las clases que se utilizan en el ejemplo:
 
## DetectorConstruction
La geometría se construye con esta clase y la configuración consiste en una envoltura con forma de caja que contiene dos volúmenes, una concha esférica y una esfera maciza, ambos sólidos se crean usando la clase G4VSolid. El material con el que están construido ambos sólidos es agua. El materiales se crea con la clase G4Material. Los volúmenes físicos están hechos de sólidos de geometría sólida constructiva (CSG) y se colocan sin rotación utilizando la clase G4PVPlacement. La clase G4PVPlacement es un volumen físico correspondiente a un solo elemento detector táctil, ubicado dentro y en relación con un volumen madre.
    
La concha esférica es de color blanco y la esfera maciza de color rojo.  Los colores se agregaron con la clase G4VisAttributes.

## PrimaryGeneratorAction
Clase virtual utilizada por el usuario para definir la generación primaria. La cinemática predeterminada es un gamma de 16.0 MeV, distribuida aleatoriamente delante de la envolvente en el 80\% del plano transversal (X, Y). Esta configuración predeterminada se puede cambiar mediante los comandos de la clase G4ParticleGun, que es un generador simple que puede disparar una o más partículas primarias.

## SteppingAction
Es en la función UserSteppingAction() donde se recolecta paso a paso la deposición de energía para un volumen seleccionado.

## EventAction
Clase de acción del usuario para ejecutar. Cree una instancia de G4Run derivada del usuario y proporcione enlaces de usuario para el inicio y el final de la ejecución.Es el evento estadístico por acumulación de eventos de deposición de energía. Al final del evento, los valores acumulados se pasan a B1RunAction y se suman durante toda la ejecución.

## RunAction 
Suma las deposiciones de energía del evento. En el modo de subprocesos múltiples, la deposición de energía acumulada en los objetos G4Accumulable por worker se fusiona con el master. La información sobre la partícula primaria se imprime en esta clase junto con el cálculo de la dosis. Actualmente, los acumulables deben registrarse en G4AccumulablesManager y G4ParametersManager::Merge().
