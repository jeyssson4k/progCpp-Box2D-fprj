## Simulacion 1

Se compila normalmente
g++ -I "./box2d/include" -L "./box2d/build/bin" s1.cpp lib.cpp -lbox2d

Se ejecuta con la siguiente instruccion
./archivo world.txt bola.txt tiempo_final showResults
ShowResults es 0 si no se desea imprimir en consola, 1 si se desea mostrar los resultados en consola
tiempo_final es la cantidad de segundos que se desea simular (como un numero entero)

## Simulacion 2

Se compila normalmente
g++ -I "./box2d/include" -L "./box2d/build/bin" s1.cpp lib.cpp -lbox2d

Se ejecuta con la siguiente instruccion
./archivo world.txt bola.txt cubo.txt config.txt edges.txt
Las configuraciones de tiempo final y showResults estan en el archivo config.txt como tf y cout respectivamente
