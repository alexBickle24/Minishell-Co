# PROYECTO MINISHELL

## INDICE
```
  1. GIT
  2. SEÑALES
```

## 1. GIT
Para trabajar en git vamos a crear ramas, veamos como realizar este proceso
```
Paso 1: Clonar Repositorio
    Siempre trabajaremos clonando este repositorio
Paso 2: Crear rama
   git branch example
   Con esto creamos nuestra rama example
Paso 3: Movernos a la rama
   git checkout example
   Estamos ahora en la rama example y procedemos a avanzar nuestro proyecto
Paso 4: Guardamos cambios
   git add . / git commit
   Hemos guardado los cambios de nuestra rama
Paso 5: Nos movemos a la rama main
   git checkout main
   Estamos en la rama main. Para ver las ramas podemos usar git branch
Paso 6: Movemos nuestro trabajo a la rama main
   git merge example
   Hemos movido los cambios de example a la main
Paso7: Destruir rama example
   git branch -d example
   Hemos cerrado la rama example porque hemos terminado la función
Paso 8: Pushear los cambios
   git push
   Guardamos el proyecto para poder visualizarlo en Github
```

## 2. SEÑALES
Con las señales vamos a crear una variable global: g_signal. Al comenzar el programa iniciamos las señales con ft_signal_init().
Esta funcion se va a encargar de detectar las señales que entran por teclado o procesos hijos y manejarlas, para ello tenemos:
```
1. S_INIT: Señal base
2. SIGINT: (Ctrl + c) Interrupción
3. SIGQUIT: (Ctrl + \) Abortar
4. SIGTSTP: (Ctrl + z) Suspender --> No creo que haga falta
5. SIGCHLD: Procesos hijos --> No se como funciona
```
