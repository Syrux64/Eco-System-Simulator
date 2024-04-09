
# 2D Eco-System Simulator!!
A window of forest for rabbits 🐰 and foxes 🦊 to inhabit in. Foxes hunt rabbits while rabbits flee. The stats
about the simlation is observed at the end. 

## Demo
![EcoSystemSimulationGIF](https://github.com/Syrux64/Eco-System-Simulator/assets/118998822/2398c519-63d1-4a58-9c21-ddd0e9c7cdc1)

## To Compile
Use the make file

```bash
  mingw32-make
```

#### OR
Run the command

```bash
  g++ -I src/include -L src/lib -o EcoSystemSimulator *.cpp src/userIncludes/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -static-libstdc++
```

## NOTE
The code is not well structured in a way for modularity/object oriented programming.

;)

