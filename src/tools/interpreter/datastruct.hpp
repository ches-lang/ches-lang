#pragma once


namespace ches::run {
    template<class T>

    struct Stack {
    private:
        size_t length = 0;

        T *data;

        void changeSize(size_t length) {
            this->data = (T*)realloc(this->data, sizeof(T) * length);
        }

    public:
        Stack() {
            this->data = (T*)malloc(sizeof(T) * 0);
        }

        void clear() {
            while(this->length > 0)
                this->pop();
        }

        bool empty() {
            return this->length == 0;
        }

        void pop() {
            if(this->empty())
                return;

            this->changeSize(this->length--);
        }

        void print() {
            if(this->empty()) {
                Console::writeln("no data.");
                return;
            }

            for(int i = 0; i < this->length; i++)
                Console::writeln(std::to_string(i) + ": " + std::to_string(this->data[i]));
        }

        void push(T value) {
            this->changeSize(this->length + 1);
            this->data[this->length] = value;
            this->length++;
        }

        size_t size() {
            return this->length;
        }

        T top() {
            return this->data[this->length];
        }
    };
}
