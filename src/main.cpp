#include <iostream>
#include <utility>

using namespace std;

class Toy {
private:
    string name;
public:
    explicit Toy(string inName) : name(std::move(inName)) {}

    Toy() : Toy("Some toy") {}
};

class shared_ptr_toy {
private:
    Toy *obj = nullptr;
    int *counter = nullptr;
    bool isEmpty = false;

    void decrement_refs() {
        if (!isEmpty) {
            isEmpty = true;
            if (counter != nullptr) {
                (*counter)--;
                if (*counter == 0) {
                    delete obj;
                    delete counter;
                }
            }
            obj = nullptr;
            counter = nullptr;
        }
    }

public:

    explicit shared_ptr_toy(Toy &toy) {
        counter = new int(1);
        obj = new Toy(toy);
    }

    explicit shared_ptr_toy(const string &inName) {
        counter = new int(1);
        obj = new Toy(inName);
    }

    shared_ptr_toy() : shared_ptr_toy("Some toy") {}

    ~shared_ptr_toy() {
        decrement_refs();
    }

    shared_ptr_toy(const shared_ptr_toy &other) {
        obj = other.obj;
        counter = other.counter;
        if (counter != nullptr) {
            (*counter)++;
        }
    }

    shared_ptr_toy &operator=(const shared_ptr_toy &other) {
        if (this == &other) {
            return *this;
        }

        if (obj != nullptr) {
            this->reset();
        }

        isEmpty = other.isEmpty;
        if (!isEmpty) {
            obj = other.obj;
            counter = other.counter;
            if (counter != nullptr) {
                (*counter)++;
            }
        }

        return *this;
    }

    void reset() {
        decrement_refs();
    }

    Toy *get() {
        return obj;
    }
};

static shared_ptr_toy make_shared_toy(const string &name) {
    return shared_ptr_toy(name);
}

static shared_ptr_toy make_shared_toy(Toy &toy) {
    return shared_ptr_toy(toy);
}

int main() {
    Toy *bone = new Toy("Bone");
    auto shared_bone = make_shared_toy(*bone);
    auto shared_bone1 = shared_ptr_toy(shared_bone);
    shared_bone.reset();

    auto shared_ball = make_shared_toy("Ball");
    auto shared_ball1 = shared_ball;

    shared_ball.reset();
    shared_ball.reset();
    shared_ball.reset();


    return 0;
}
