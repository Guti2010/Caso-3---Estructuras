#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "book.h"

std::vector<Book> generateLibrary() {
    std::vector<Book> library;

    // Libro: A Christmas Carol
    Book book1;
    book1.title = "A Christmas Carol";
    book1.chaptersBySentiment["Avaricia"] = {"Stave One"};
    book1.chaptersBySentiment["Remordimiento"] = {"Stave Two"};
    book1.chaptersBySentiment["Redención"] = {"Stave Three"};
    book1.chaptersBySentiment["Preocupación"] = {"Stave Four"};
    book1.chaptersBySentiment["Felicidad/Regocijo"] = {"Stave Five"};
    library.push_back(book1);

    // Libro: A Doll's House
    Book book2;
    book2.title = "A Doll's House";
    book2.chaptersBySentiment["Engaño"] = {"Acto I"};
    book2.chaptersBySentiment["Revelación"] = {"Acto II"};
    book2.chaptersBySentiment["Liberación"] = {"Acto III"};
    library.push_back(book2);

    // Libro: A Modest Proposal
    Book book3;
    book3.title = "A Modest Proposal";
    book3.chaptersBySentiment["Ironía"] = {"Sección Única"};
    library.push_back(book3);

    // Libro: A Tale of Two Cities
    Book book4;
    book4.title = "A Tale of Two Cities";
    book4.chaptersBySentiment["Contrastes"] = {"Libro Primero"};
    book4.chaptersBySentiment["Desesperación"] = {"Libro Segundo"};
    book4.chaptersBySentiment["Sacrificio"] = {"Libro Tercero"};
    library.push_back(book4);

    // Libro: Adventures of Huckleberry Finn
    Book book5;
    book5.title = "Adventures of Huckleberry Finn";
    for (int i = 1; i <= 11; ++i) {
        book5.chaptersBySentiment["Aventura"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 12; i <= 22; ++i) {
        book5.chaptersBySentiment["Amistad"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book5);

    // Libro: Alice's Adventures in Wonderland
    Book book6;
    book6.title = "Alice's Adventures in Wonderland";
    for (int i = 1; i <= 6; ++i) {
        book6.chaptersBySentiment["Asombro"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 7; i <= 12; ++i) {
        book6.chaptersBySentiment["Confusión"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book6);

    // Libro: Dracula
    Book book7;
    book7.title = "Dracula";
    for (int i = 1; i <= 7; ++i) {
        book7.chaptersBySentiment["Intriga"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 8; i <= 14; ++i) {
        book7.chaptersBySentiment["Terror"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 15; i <= 27; ++i) {
        book7.chaptersBySentiment["Persecución"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 28; i <= 37; ++i) {
        book7.chaptersBySentiment["Confrontación"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book7);

    // Libro: Frankenstein
    Book book8;
    book8.title = "Frankenstein";
    book8.chaptersBySentiment["Preocupación"] = {"Cartas"};
    book8.chaptersBySentiment["Creación"] = {"Volumen I"};
    book8.chaptersBySentiment["Rechazo"] = {"Volumen II"};
    book8.chaptersBySentiment["Venganza"] = {"Volumen III"};
    library.push_back(book8);

    // Libro: Great Expectations
    Book book9;
    book9.title = "Great Expectations";
    book9.chaptersBySentiment["Ambición"] = {"Parte 1"};
    book9.chaptersBySentiment["Arrepentimiento"] = {"Parte 2"};
    book9.chaptersBySentiment["Redención"] = {"Parte 3"};
    library.push_back(book9);

    // Libro: Jane Eyre: An Autobiography
    Book book10;
    book10.title = "Jane Eyre: An Autobiography";
    for (int i = 1; i <= 11; ++i) {
        book10.chaptersBySentiment["Soledad"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 12; i <= 27; ++i) {
        book10.chaptersBySentiment["Amor"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book10);

    // Libro: Metamorphosis
    Book book11;
    book11.title = "Metamorphosis";
    book11.chaptersBySentiment["Sorpresa"] = {"Capítulo 1"};
    book11.chaptersBySentiment["Aislamiento"] = {"Capítulo 2"};
    book11.chaptersBySentiment["Aceptación"] = {"Capítulo 3"};
    library.push_back(book11);

    // Libro: Moby Dick; Or, The Whale
    Book book12;
    book12.title = "Moby Dick; Or, The Whale";
    for (int i = 1; i <= 39; ++i) {
        book12.chaptersBySentiment["Obsesión"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 40; i <= 71; ++i) {
        book12.chaptersBySentiment["Perseverancia"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book12);

    // Libro: Pride and Prejudice
    Book book13;
    book13.title = "Pride and Prejudice";
    for (int i = 1; i <= 23; ++i) {
        book13.chaptersBySentiment["Prejuicio"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 24; i <= 61; ++i) {
        book13.chaptersBySentiment["Amor"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book13);

    // Libro: Romeo and Juliet
    Book book14;
    book14.title = "Romeo and Juliet";
    book14.chaptersBySentiment["Enamoramiento"] = {"Acto I"};
    book14.chaptersBySentiment["Romance"] = {"Acto II"};
    book14.chaptersBySentiment["Tragedia"] = {"Acto III"};
    library.push_back(book14);

    // Libro: The Adventures of Sherlock Holmes
    Book book15;
    book15.title = "The Adventures of Sherlock Holmes";
    book15.chaptersBySentiment["Desconocido"] = {"Este libro de cuentos sigue la narrativa de diferentes casos y no tiene una división clara en capítulos con emociones predominantes."};
    library.push_back(book15);

    // Libro: The Great Gatsby
    Book book16;
    book16.title = "The Great Gatsby";
    for (int i = 1; i <= 3; ++i) {
        book16.chaptersBySentiment["Aspiración"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 4; i <= 9; ++i) {
        book16.chaptersBySentiment["Desilusión"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book16);

    // Libro: The Iliad
    Book book17;
    book17.title = "The Iliad";
    for (int i = 1; i <= 12; ++i) {
        book17.chaptersBySentiment["Guerra"].push_back("Canto " + std::to_string(i));
    }
    for (int i = 13; i <= 24; ++i) {
        book17.chaptersBySentiment["Duelo"].push_back("Canto " + std::to_string(i));
    }
    library.push_back(book17);

    // Libro: The Importance of Being Earnest
    Book book18;
    book18.title = "The Importance of Being Earnest";
    book18.chaptersBySentiment["Ironía"] = {"Acto I"};
    book18.chaptersBySentiment["Revelación"] = {"Acto II"};
    library.push_back(book18);

    // Libro: The Legend of Sleepy Hollow
    Book book19;
    book19.title = "The Legend of Sleepy Hollow";
    book19.chaptersBySentiment["Suspenso"] = {"Este relato no sigue una estructura de capítulos."};
    library.push_back(book19);

    // Libro: The Picture of Dorian Gray
    Book book20;
    book20.title = "The Picture of Dorian Gray";
    for (int i = 1; i <= 6; ++i) {
        book20.chaptersBySentiment["Belleza"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 7; i <= 12; ++i) {
        book20.chaptersBySentiment["Corrupción"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book20);

    // Libro: The Prince
    Book book21;
    book21.title = "The Prince";
    book21.chaptersBySentiment["Estrategia"] = {"Capítulos"};
    library.push_back(book21);

    // Libro: The Scarlet Letter
    Book book22;
    book22.title = "The Scarlet Letter";
    for (int i = 1; i <= 8; ++i) {
        book22.chaptersBySentiment["Culpa"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 9; i <= 24; ++i) {
        book22.chaptersBySentiment["Redención"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book22);

    // Libro: The Souls of Black Folk
    Book book23;
    book23.title = "The Souls of Black Folk";
    book23.chaptersBySentiment["Desconocido"] = {"Este libro no sigue una estructura de capítulos clásica, ya que se compone de ensayos y reflexiones."};
    library.push_back(book23);

    // Libro: The Strange Case of Dr. Jekyll and Mr. Hyde
    Book book24;
    book24.title = "The Strange Case of Dr. Jekyll and Mr. Hyde";
    for (int i = 1; i <= 5; ++i) {
        book24.chaptersBySentiment["Misterio"].push_back("Capítulo " + std::to_string(i));
    }
    for (int i = 6; i <= 10; ++i) {
        book24.chaptersBySentiment["Horror"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book24);

    // Libro: The Yellow Wallpaper
    Book book25;
    book25.title = "The Yellow Wallpaper";
    book25.chaptersBySentiment["Locura"] = {"Historia completa"};
    library.push_back(book25);

    // Libro: The young master of Hyson Hall
    Book book26;
    book26.title = "The young master of Hyson Hall";
    book26.chaptersBySentiment["Aventura"] = {"Libro"};
    library.push_back(book26);

    // Libro: Toodle and Noodle Flat-Tail
    Book book27;
    book27.title = "Toodle and Noodle Flat-Tail";
    book27.chaptersBySentiment["Aventura"] = {"Libro"};
    library.push_back(book27);

    Book book28;
    book28.title = "The Federalist Papers";
    for (int i = 1; i <= 85; ++i) {
        book28.chaptersBySentiment["Política"].push_back("Ensayo " + std::to_string(i));
    }
    library.push_back(book28);

    // Trigésimo libro: War and Peace
    Book book29;
    book29.title = "War and Peace";
    for (int i = 1; i <= 361; ++i) {
        book29.chaptersBySentiment["Guerra"].push_back("Capítulo " + std::to_string(i));
    }
    library.push_back(book29);

    // Trigésimo primer libro: The Odyssey
    Book book30;
    book30.title = "The Odyssey";
    for (int i = 1; i <= 24; ++i) {
        book30.chaptersBySentiment["Aventura"].push_back("Canto " + std::to_string(i));
    }
    library.push_back(book30);

    return library;

}