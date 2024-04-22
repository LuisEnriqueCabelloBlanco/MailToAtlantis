# Como escribir eventos en el npcData.json

Los eventos tienen un numero de paquetes que spawnearan a lo largo del día, y un numero de paquetes necesarios para cumplir la mision. Estos dos numeros no hacen falta que sean el mismo, pueden spawnear 10 paquetes y solo necesitar 2, por ejemplo.

Para los eventos, DEBEMOS establecer 3 secciones en el json:
1- Paquetes: que incluira como van a crearse los paquetes y podremos especificar uno por uno o poner características generales y que se generen todos iguales.
2- Condiciones: las condiciones que revisará el tubo para saber si el paquete procesado sirve para la mision o no
3- Recompensas: Si se cumple la misión, que personaje recibirá felicidad, etc

Ejemplo: *"Redirige 3 paquetes que saldrán de Hestia de comida mandados por Paco Fiestas y mándalos a Demeter"*

```
    "1":{
        "numPaquetes": 3,
        "numPaquetesParaCompletar": 3,
        "paquetes":{
            "remitente": "Paco Fiestas",
            "distrito": "Hestia",
            "tipoPaquete": "Alimentos"
          }
        },
        "condiciones":{
            "remitente": "Paco Fiestas",
            "distrito": "Demeter",
            "tipoPaquete": "Alimentos"
          
        },
        "recompensa": {
          "1": "Vagabundo+",
          "2": "Secretario-"
        }
      },
```
(Escribirlas tal cual)
##### Características que podemos poner en la sección paquetes:
remitente, distrito, calle, tipoPaquete, peso, pesoKG, fragil, legal
##### Características que podemos poner en la sección condiciones:
remitente, distrito, calleMarcada, tipoPaquete, peso, pesoKG, fragil, legal
##### Recopmensas que poder poner en la sección recompensas:
(Personaje)+ o - 
El + es para subir un nivel de felicidad y el menos para bajar uno, se pueden poner mas de un + o -.

Ejemplo: Vagabundo+++ (Vagabundo + 3 niveles de felicidad)



# IMPORTANTE Como escribir los datos
Los datos que nos dejemos sin rellenar se pondrán random según el día. Hay dos formas de escribir datos, generales para todos los paquetes, o especificos para cada paquete:

###### Para todos los paquetes los mismos datos
```
"numPaquetes: 3
"paquetes":{
    "remitente": "Paco Fiestas"
},
"condiciones":{
    "distrito": "Poseidon",
    "calleMarcada": "C2"
},
"recompensa": "Vagabundo+"
```
###### Para cada paquetes unas caracteristicas y condiciones especiales
```
"numPaquetes: 2
"paquetes":{
    "1":{
        "remitente": "Paco Fiestas"
    },
    "2":{
        "remitente": "Jake Perro"
    }
    
},
"condiciones":{
    "1":{
        "remitente": "Paco Fiestas"
        "distrito": "Poseidon",
        "calleMarcada": "C2"
    },
    "2":{
        "remitente": "Jake Perro",
        "distrito": "Erroneo"
    }
    
},
"recompensa": {
    "1": "Vagabundo+",
    "2": "Secretario-"
}
```

Cosas a tener en cuenta:
Se puede poner solo una recompensa para ello, poner "recompensa": "(recompensa)" en vez de "recompensa"{
    "1": ...
    "2": ...
}

DUDAS A MIGUEL 🐐