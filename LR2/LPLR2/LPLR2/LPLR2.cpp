#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;

const int WIDTH = 100;   // розмір області по X
const int HEIGHT = 30;   // розмір області по Y
const int SPEED = 1;     // швидкість переміщення (в одиницях координат)
const int DELAY_MS = 100; // затримка між кроками (100 мс)

mutex printMutex;

class Fish {
public:
    string name;
    int x, y;
    int directionX, directionY;

    Fish(string name, int x, int y, int dx, int dy)
        : name(name), x(x), y(y), directionX(dx), directionY(dy) {
    }

    void moveX() {
        while (true) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << name << " at X=" << x << endl;
            }

            x += directionX * SPEED;
            if (x >= WIDTH || x <= 0) {
                directionX *= -1;
            }

            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }

    void moveY() {
        while (true) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << name << " at Y=" << y << endl;
            }

            y += directionY * SPEED;
            if (y >= HEIGHT || y <= 0) {
                directionY *= -1;
            }

            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }
};

int main() {
    // Створимо золотих рибок (рухаються по X)
    vector<thread> threads;
    threads.emplace_back(&Fish::moveX, Fish("Goldfish_1", 0, 5, 1, 0));
    threads.emplace_back(&Fish::moveX, Fish("Goldfish_2", WIDTH, 10, -1, 0));

    // Створимо гуппі (рухаються по Y)
    threads.emplace_back(&Fish::moveY, Fish("Guppy_1", 20, 0, 0, 1));
    threads.emplace_back(&Fish::moveY, Fish("Guppy_2", 50, HEIGHT, 0, -1));

    // Очікуємо завершення потоків (вони нескінченні, тому фактично це блокування)
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
