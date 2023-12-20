#ifndef STATIC_QUEUE_H
#define STATIC_QUEUE_H

#include <ctype.h>

/**
 * Static-size circular queue class
 * 
 * Note: The actual capacity of the queue is SIZE-1 items because otherwise a
 * completely full queue is indistinguishable from a completely empty queue.
 * It's technically slightly more memory efficient to keep a boolean isFull,
 * but this seems to me to be less prone to errors.
*/
template <class T, size_t SIZE>
class static_queue {
    private:
        T queue[SIZE];
        size_t read_ind = 0;
        size_t write_ind = 0;
        inline void push_unsafe(const T& item);
        inline void pop_unsafe(T* dest);
    public:
        // Returns the number of items currently in the queue
        inline size_t size() const;
        // Returns the number of items that can be added to the queue
        inline size_t free() const;
        // Try to add an item to the queue
        // Returns true on success, false on fail
        inline bool push(const T& item);
        // Force-add an item to the queue; discard oldest item if queue is full
        // Returns true if no item was discarded, false if any were
        inline bool push_anyway(const T& item);
        // Try to remove an item from the queue if one is available
        // Returns true if successful, 
        inline bool pop(T* dest);
};

template <class T, size_t SIZE>
size_t static_queue<T, SIZE>::size() const {
    if(read_ind > write_ind) {
        return write_ind + SIZE - read_ind;
    } else {
        return write_ind - read_ind;
    }
}

template <class T, size_t SIZE>
size_t static_queue<T, SIZE>::free() const {
    return SIZE - size() - 1;
    // entirely full is indistinguishable from entirely empty, so we have to
    // leave one space
}


template <class T, size_t SIZE>
void static_queue<T, SIZE>::push_unsafe(const T& item) {
    queue[write_ind] = item;
    write_ind = (write_ind + 1) % SIZE;
}

template <class T, size_t SIZE>
void static_queue<T, SIZE>::pop_unsafe(T* dest) {
    if(dest) { *dest = queue[read_ind]; }
    read_ind = (read_ind + 1) % SIZE;
}

template <class T, size_t SIZE>
bool static_queue<T, SIZE>::push(const T& item) {
    if(free() > 0) {
        push_unsafe(item);
        return true;
    } else {
        return false;
    }
}

template <class T, size_t SIZE>
bool static_queue<T, SIZE>::push_anyway(const T& item) {
    if(free() == 0) {
        pop_unsafe(nullptr);
        push_unsafe(item);
        return false;
    } else {
        push_unsafe(item);
        return true;
    }
}

template <class T, size_t SIZE>
bool static_queue<T, SIZE>::pop(T* item) {
    if(size() > 0) {
        pop_unsafe(item);
        return true;
    } else {
        return false;
    }
}

#endif