#include <raylib.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

int main()
{
    try
    {
        SQLite::Database db(
            "cinema.db",
            SQLite::OPEN_READWRITE |
            SQLite::OPEN_CREATE
        );

        db.exec(R"(
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT
            );
        )");

        std::cout << "SQLite works!" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    InitWindow(800, 600, "Cinema Booking System");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(
            "raylib + SQLiteCpp works!",
            180,
            280,
            30,
            RAYWHITE
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}