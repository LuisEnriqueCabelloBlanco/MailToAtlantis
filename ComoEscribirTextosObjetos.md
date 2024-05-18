# Como escribir textos de objetos en el dialojos.json

Los textos deben de seguir normalmente un patrón que se repite cada 3 días.

Pongamos de ejemplo estos 3 textos que aparecen el los 3 primeros días al interactuar con el cartel de la oficina del distrito de Hestia:

- Plantilla general: Texto+(Nombre de Objeto)+(Día)+(Número aleatorio entre 0 y 1), el numero 0 o 1 es lo que indica si de manera aleatoria se pone el texto de la opción 0 o 1 de ese día.

Día 1:
	opción 0:

        "TextoCartelOficina10": [ "Aqu$i estaban buscando trabajo." ],

	opción 1:

        "TextoCartelOficina11": [ "Bonito logo el del cartel." ],

Día 2:
	opción 0:

        "TextoCartelOficina20": [
            "Aqu$i estaban buscando trabajo.",
            "Aprovech$e para cobrar algo, as$i que me apunt$e"
        ],

	opción 1:

        "TextoCartelOficina21": [ "El artista deb$ia de estar muy inspirado para hacer este logo del cartel." ],

Día 3:
	opción 0:

        "TextoCartelOficina30": [
            "Aqu$i estaban buscando trabajo.",
            "Ya no necesitan m$as personal."
        ],

	opción 1:

        "TextoCartelOficina31": [ "Espero seguir teniendo un trabajo estable." ]

En la opción 0 de los días 1, 2 y 3 (10, 20 y 30), el texto gira en torno a una misma idea común como lo es el hecho de que Paco OBSERVA que en ese sitio estaban
buscando trabajo y de ahí surgen variaciones o frases parecidas en esos 3 días. Esto de coger una idea y hacer variaciones a través de la misma es para que nadie se coma
la puta cabeza a la hora de crear textos diferentes para cada día.

Pero a veces puede romperse ese patrón un poquito, como lo es en la opción 1 de esos 3 primeros días, donde los 2 primeros se habla sobre lo bueno que es el artista del logo, pero en el tercero se habla de querer tener un trabajo estable. No siempre debe de seguirse el patrón que se menciona al inicio, pero es aconsejable no desviarse mucho para mantener algo de consistencia.

Lo que si se debe de seguir es que se debe de escribir sobre lo que se VE de ESE OBJETO (si por ejemplo vemos un árbol sin manzanas, no podemos decir qué ricas deben de estar esas manzanas, pero sí que se puede decir que a lo mejor ese árbol tiene ADENTRO manzanas que no se ven a simple vista o que ese árbol le recuerda a cualquier cosa del escenario, distrito o recuerdo que haya visto Paco (se pueden inventar recuerdos de Paco mientras no se rompa mucho con el lore actual)).

Los nombres de los objetos que hay en el json de diálogos están indicados en los mapas que hay en la misma carpeta de este documento (en formato .png), y sería ideal encubrir lo que se ve en los cuadros rojos de cada objeto.

Cualquier cosa comparen lo que acabo de explicar con cómo están los otros textos de objetosa escritos (no vean los textos de la CasGrande de Hestia como referencia, esos los debe de cambiar el putero).

DUDAS A JOSÉ PUTERO.