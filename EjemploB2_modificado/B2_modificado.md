# Modificaciones ejemplo B2

## B2aDetectorConstructor
La configuración del detector consiste en un objetivo seguido de dos cámaras de tamaño transversal creciente. Las cámaras están construidas con cilindros simples y están representadas en color rojo. Están ubicadas en la región llamada Tracker, esta región tiene forma esférica y está representada en color azul. Para poder utilizar sólidos esféricos se agregó “#include G4Sphere.hh”. El target tiene forma cilíndrica y está representado en color morado.      
Se realizaron cambios en las dimensiones de los sólidos y en el material del tracker, se cambió aire por tejido.
    
## B2PrimaryGeneratorAction 
Se realizaron tres pruebas, en las primeras dos las partículas primarias fueron protones con valores de energía distintos mientras que en la tercera se utilizaron neutrones. Las energías de los primeros protones corresponden a 1GeV y la de los segundos a 10GeV. Todas las pruebas se hicieron en presencia de un campo magnético de 0.2T a lo largo del eje horizontal. En cada caso la cinemática primaria consiste en tres partículas, disparadas una a la vez, que  golpean el objetivo perpendicular a la cara de entrada.La lista de los procesos físicaos no se cambió, requiere archivos de datos para procesos electromagnéticos y hadrónicos.
