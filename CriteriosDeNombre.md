# Convenio de Programación
## De forma general
Un codigo no se considera cerrado / terminado si no tiene comentarios en él.
**NO USAR**:
- tíldes 
- la ñ
- símbolos no ASCCI

## Nombres
Clases
: Comenzando en mayúsculas y haciendo camel case. No emplear la barra baja y si se se está empleando un patrón de diseño añadirlo en el nombre

Variables
: Dependiendo de donde se declaren seguiran un criterio u otro pero siempre con un nombre explicativo o minimamente descriptivo
: - Atributos privados de la clase
    : comenzando en minúscula y siguiendo camel case. Se teminará el nombre con barra baja.
: - Referencias a componentes de la misma entidad
    : m[nombre acortado]_ siguiendo camel case después de la 'm'
    : Aclaraciones y ejemplos
        : mTr_ para Transform y mTrg_ para trigger
        : mRend_ para RenderImage
: - Variables de ambito de una función
    : Comenzando en minuscula y siguiendo camel case.

Constantes
: Se escribe en mayusculas y con la barra baja como separador entre palabras

Métodos
: Comenzando en minuscula y siguiendo con camel case siempre con un nombre descriptivo de la tarea que van a realizar

## Comentarios
En el .h se harán los comentarios generales con el <'summary'> que permite usar ademas etiquetas XML por si a alguno le resulta útil.
Los comentarios siempre se escriben encima del código si es el .h en el caso del cpp queda un poco más a discrecion ya que es para cuestiones informativas de implementación y no cuestión de la interfaz.

Clases
: Los comentarios sobre una clase deben explicar mas o menos su utilidad y si tiene dependencias o necesidades especiales para ser usada

Variables
: Se aplica el criterio genral de comentarios

Métodos
: Los comentarios sobre metodos deben explicar que hace el método sin entrar en su implementación directa preferiblemente. Además es recomendable emplear los comentarios de parametros para facilitar su comprensión

## Uso del código

Las clases tendrán primero sus atributos y métodos públicos y luego sus atributos protegidos y por último los privados. En cada una de esos apartados se escribirán primero los métodos y luego las variables.

Intentar mantener los tipos enumerados en un espacio de nombres para mayor facilidad de uso si va a tener un uso mayor que el de la propia clase.


## Ejemplos
```
class Foo{
public:

protected:

private:
}
```

## Keys de audio

Los sonidos (todos, incluso aquellos que solo van a tener una pull de 1) deben tener un número al final de su nombre base, empezando por el cero de manera ascendente. Por ejemplo, los sonidos del paquete contra la mesa podrían ser llamados 'choque' y sus diferentes keys "choque0", "choque1", "choque2", etc.
