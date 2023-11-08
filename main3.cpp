#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Nombres de los archivos de libros definidos
    std::string nombresLibros[] = {
        "A Christmas Carol in Prose; Being a Ghost Story of Christmas.txt", "A Doll's House  a play.txt", "The Legend of Sleepy Hollow.txt", "A Tale of Two Cities.txt", "Adventures of Huckleberry Fin.txt", 
        "Alice's Adventures in Wonderl.txt","Brothers in arms.txt","Dracula.txt","Field artillery materiel.txt","Frankenstein.txt","Great Expectations.txt",
        "Grimms' Fairy Tales.txt", "Jane Eyre An Autobiography.txt","Metamorphosis.txt", "Moby Dick; Or, The Whale.txt", "Pride and Prejudice.txt",
        "Romeo and Juliet.txt","he Adventures of Sherlock Homes.txt","The Great Gatsby.txt", "The Iliad.txt", "The Importance of Being Earne.txt",
        "The Legend of Sleepy Hollow.txt", "The Picture of Dorian Gray.txt","The Prince.txt","The Scarlet Letter.txt","The Souls of Black Folk.txt",
        "The Strange Case of Dr. Jekyl.txt","The Yellow Wallpaper.txt", "The young master of Hyson Hal.txt", "Toodle and Noodle Flat-tail T.txt"

        // Agrega más nombres de archivos según sea necesario

    };

    for (const std::string& nombreLibro : nombresLibros) {
        // Ruta completa al archivo de texto
        std::string rutaArchivo = "Libros/" + nombreLibro;

        // Abre el archivo de texto para lectura
        std::ifstream archivo(rutaArchivo);

        if (archivo.is_open()) {
            std::string contenidoLibro;
            std::string linea;

            // Lee el contenido del archivo línea por línea
            while (std::getline(archivo, linea)) {
                contenidoLibro += linea + "\n";
            }

            // Procesa el contenido del libro según sea necesario
            // Por ejemplo, puedes realizar análisis o búsqueda en el contenido aquí

            // Imprime el contenido del libro
            std::cout << "Contenido de " << nombreLibro << ":\n" << contenidoLibro << "\n";

            archivo.close(); // Cierra el archivo
        } else {
            std::cerr << "No se pudo abrir el archivo: " << rutaArchivo << std::endl;
        }
    }

    return 0;
}


