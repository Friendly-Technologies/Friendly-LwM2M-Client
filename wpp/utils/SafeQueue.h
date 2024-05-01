/**
 * @file SafeQueue.h
 * @version 1.0.1
 * @ref https://github.com/VSkoshchuk/SafeQueue
 * 
 * Copyright (c) 2023 VSkoshchuk skoschuk999@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SAFE_QUEUE_H_
#define SAFE_QUEUE_H_


#include <string.h>
#include <vector>


/**
 * @brief Include with the guard implementation for protecting the critical section of code.
 * @note Can be changed to any other implementation.
 */
#include <WppGuard.h>
/**
 * @def SQ_GUARD_CREATE(name)
 * @brief Macro for creating a guard object with the specified name.
 * The guard object is used to protect critical sections of code from simultaneous access by multiple threads.
 * The name parameter should be a valid identifier.
 * @note Can be changed to any other implementation.
 */
#define SQ_GUARD_CREATE(name)		wpp::WppGuard name
/**
 * @def SQ_GUARD_LOCK(name)
 * @brief Macro for locking the specified guard object.
 * The lock() method is used to acquire the lock on the guard object, preventing other threads from accessing the critical section of code.
 * The name parameter should be the name of a guard object.
 * @note Can be changed to any other implementation.
 */
#define SQ_GUARD_LOCK(name)			name.lock()
/**
 * @def SQ_GUARD_UNLOCK(name)
 * @brief Macro for unlocking the specified guard object.
 * The unlock() method is used to release the lock on the guard object, allowing other threads to access the critical section of code.
 * The name parameter should be the name of a guard object.
 * @note Can be changed to any other implementation.
 */
#define SQ_GUARD_UNLOCK(name)		name.unlock()
/**
 * @def SQ_GUARD_TRY_LOCK(name)
 * @brief Macro for trying to lock the specified guard object.
 * The try_lock() method is used to attempt to acquire the lock on the guard object.
 * If the lock is successfully acquired, the method returns true. Otherwise, it returns false.
 * The name parameter should be the name of a guard object.
 * @note Can be changed to any other implementation.
 */
#define SQ_GUARD_TRY_LOCK(name)		name.try_lock()


#define SAFE_QUEUE_DEF_ELEM_CNT	0x01


/**
 * @brief SafeQueue is an implementation of the thread/IRQ safe queue that does not
 * use dynamic memory, and can perform sumultaneously pop() and push() without any
 * blocking, that alows use this implementation as data container in the critical
 * sections, or threads and IRQ where it is impossible to wait for access to the 
 * container and data loss is unacceptable. Developed with a focus on embedded systems.
 * 
 * The problem it solves is the following: it is necessary to have a container that
 * allows adding and removing elements independently by different streams/ISR. This
 * container is protected from the simultaneous call of the push/pop method from
 * the same threads, and allows the simultaneous call of push and pop from different
 * threads without internal blocking, that is, if an interruption occurred that interrupted
 * the removal process, it can easily perform addition or vice versa. For example, if
 * there is an interrupt in which you need to add an element to the queue and there is
 * a main loop that removes elements from the queue, if you use standard containers, then
 * to use them in the main loop, you must first ensure access synchronization so that the
 * internal state is not changed if an interruption occurs, but in this case, if an
 * interrupt occurs at this time, it will not be able to access the container until the
 * main loop releases the mutex, which in turn leads to data loss.
 */
template <typename T, size_t SIZE>
class SafeQueue {
public:
	SafeQueue() {}

	/**
	 * @brief Adds an element or an array of elements to the queue.
	 * The push() method can be simultaneously called with the pop() method from different threads,
	 * but simultaneous calls to push() from the same thread are prohibited and protected by the guard.
	 * Adding an element(s) to the queue is done via a byte-by-byte copy, so the data type must be simple.
	 * 
	 * @param data - Pointer to the data to be added to the queue.
	 * @param elements_cnt - Number of elements to be added to the queue if data is an array.
	 * 
	 * @return true if the data was successfully added to the queue, false otherwise.
	 */ 
	bool push(const T * const data, size_t elements_cnt = SAFE_QUEUE_DEF_ELEM_CNT);

	/**
	 * @brief Removes a specified number of elements from the queue.
	 * The pop() method can be simultaneously called with the push() method from different threads,
	 * but simultaneous calls to pop() from the same thread are prohibited and protected by the guard.
	 * 
	 * @param elements_cnt - Number of elements to be removed from the queue.
	 * 
	 * @return true if the elements were successfully removed from the queue, false otherwise.
	 */ 
	bool pop(size_t elements_cnt = SAFE_QUEUE_DEF_ELEM_CNT);

	/**
	 * @brief Converts the contents of the queue to a vector.
	 * The to_vector() method creates a vector containing a specified number of elements from the queue.
	 * 
	 * @note Can not be simultaneously called with pop(), to_vector(), front(T*), back(T*), at(T*).
	 * 
	 * @param elements_cnt - Number of elements to be included in the vector.
	 * 
	 * @return A vector containing the specified number of elements from the queue.
	 */ 
	std::vector<T> to_vector(size_t elements_cnt = SIZE);

	/**
	 * @brief Returns a pointer to the first element in the queue.
	 * The front() method does not guarantee that the pointer
	 * to the data after the return will still be correct if after
	 * or during this call will be called pop().
	 * 
	 * @return A pointer to the first element in the queue.
	 */
	T* front();

	/**
	 * @brief Returns a pointer to the last element in the queue.
	 * The back() method does not guarantee that the pointer
	 * to the data after the return will still be correct if after
	 * or during this call will be called pop().
	 * 
	 * @return A pointer to the last element in the queue.
	 */
	T* back();

	/**
	 * @brief Returns a pointer to the element at the specified index in the queue.
	 * The at() method does not guarantee that the pointer
	 * to the data after the return will still be correct if after
	 * or during this call will be called pop().
	 * 
	 * @param i - The index of the element.
	 * 
	 * @return A pointer to the element at the specified index in the queue.
	 */
	T* at(size_t i);

	/**
	 * @brief Returns the first element in the queue and copies it to the specified memory location.
	 * The front() method can guarantee the integrity of the data and ensures that it corresponds to what was written to the queue.
	 * The user needs to pass a pointer to the memory allocated for this type.
	 * 
	 * @note Can not be simultaneously called with pop(), to_vector(), front(T*), back(T*), at(T*).
	 * 
	 * @param data - Pointer to the memory location where the first element will be copied.
	 * 
	 * @return true if the first element was successfully copied to the specified memory location, false otherwise.
	 */
	bool front(T* data);

	/**
	 * @brief Returns the last element in the queue and copies it to the specified memory location.
	 * The back() method can guarantee the integrity of the data and ensures that it corresponds to what was written to the queue.
	 * The user needs to pass a pointer to the memory allocated for this type.
	 * 
	 * @note Can not be simultaneously called with pop(), to_vector(), front(T*), back(T*), at(T*).
	 * 
	 * @param data - Pointer to the memory location where the last element will be copied.
	 * 
	 * @return true if the last element was successfully copied to the specified memory location, false otherwise.
	 */
	bool back(T* data);

	/**
	 * @brief Returns the element at the specified index in the queue and copies it to the specified memory location.
	 * The at() method can guarantee the integrity of the data and ensures that it corresponds to what was written to the queue.
	 * The user needs to pass a pointer to the memory allocated for this type.
	 * 
	 * @note Can not be simultaneously called with pop(), to_vector(), front(T*), back(T*), at(T*).
	 * 
	 * @param i - The index of the element.
	 * @param data - Pointer to the memory location where the element will be copied.
	 * 
	 * @return true if the element at the specified index was successfully copied to the specified memory location, false otherwise.
	 */
	bool at(size_t i, T* data);

	/**
	 * @brief Clears the queue.
	 * 
	 * @note Can not be simultaneously called with pop(), to_vector(), front(T*), back(T*), at(T*).
	 */
	void clear();

	/**
	 * @brief Checks if the queue is empty.
	 * 
	 * @return true if the queue is empty, false otherwise.
	 */
	bool is_empty();

	/**
	 * @brief Checks if the queue is full.
	 * 
	 * @return true if the queue is full, false otherwise.
	 */
	bool is_full();

	/**
	 * @brief Returns the current size of the queue.
	 * 
	 * @return The current size of the queue.
	 */
	size_t size();

	/**
	 * @brief Returns the number of additional elements the queue can hold.
	 *
	 * Calculates the remaining capacity of the queue, which is the difference
	 * between the queue's maximum capacity and its current size.
	 *
	 * @return Available space in the queue.
	 */
	size_t available_space();

	/**
	 * @brief Returns the size of each element in the queue.
	 * 
	 * @return The size of each element in the queue.
	 */
	uint32_t element_size();
	
private:
	/**
	 * @brief Before calling these method, validate items count in the queue
	 */
	inline size_t _get_next_index(size_t base_index, size_t elements_cnt = SAFE_QUEUE_DEF_ELEM_CNT);
	inline size_t _get_prev_index(size_t base_index, size_t elements_cnt = SAFE_QUEUE_DEF_ELEM_CNT);

	inline bool _check_remove_ability(size_t elements_cnt = SAFE_QUEUE_DEF_ELEM_CNT);
	inline bool _check_add_ability(size_t elements_cnt = SAFE_QUEUE_DEF_ELEM_CNT);

private:
	SQ_GUARD_CREATE(_push_guard);
	SQ_GUARD_CREATE(_pop_guard);
	size_t _front_i = 0x00, _end_i = 0x00, _counter = 0x00;
	T _buffer[SIZE];
};


/* ------------- Public methods -------------*/

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::push(const T * const data, size_t elements_cnt) {
	if (!SQ_GUARD_TRY_LOCK(_push_guard)) return false;

	/* Check _buffer available space */
	if (!elements_cnt || _check_add_ability(elements_cnt) == false) {
		SQ_GUARD_UNLOCK(_push_guard);
		return false;
	}

	for (size_t i = 0; i < elements_cnt; i++) {
		memcpy((uint8_t *)(_buffer + _end_i), (uint8_t *)(data + i), sizeof(T));
		_end_i = _get_next_index(_end_i);
		_counter++;
	}

	SQ_GUARD_UNLOCK(_push_guard);
	return true;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::pop(size_t elements_cnt) {
	if (!SQ_GUARD_TRY_LOCK(_pop_guard)) return false;

	/* Maybe the _buffer available elements not enough */
	if (_check_remove_ability(elements_cnt) == false) { 
		SQ_GUARD_UNLOCK(_pop_guard);
		return false;
	}

	_front_i = _get_next_index(_front_i, elements_cnt);
	_counter -= elements_cnt;

	SQ_GUARD_UNLOCK(_pop_guard);
	return true;
}

template <typename T, size_t SIZE>
std::vector<T> SafeQueue<T, SIZE>::to_vector(size_t elements_cnt) {
	SQ_GUARD_LOCK(_pop_guard);

	/* Maybe the _buffer available elements not enough */
	if (!elements_cnt || elements_cnt > size()) {
		SQ_GUARD_UNLOCK(_pop_guard);
		return std::vector<T>();
	}

	size_t end_of_range_index = _get_next_index(_front_i, elements_cnt);
	uint8_t is_out_of_range = _front_i >= end_of_range_index;

	/**
	 * @brief Available two potential situations:
	 * 1) Elements range placed between _front_i and SIZE
	 * 2) Elements range placed between _front_i -> SIZE and 0 -> end_of_range_index
	 *
	 * If is_out_of_range set to true then we have second situation,
	 * and converting operation will be performed in two steps
	 */
	std::vector<T> converted (_buffer + _front_i, (is_out_of_range)? _buffer + SIZE : _buffer + end_of_range_index);
	if (is_out_of_range) converted.insert(converted.end(), _buffer, _buffer + end_of_range_index);

	SQ_GUARD_UNLOCK(_pop_guard);
	return converted;
}

template <typename T, size_t SIZE>
T* SafeQueue<T, SIZE>::front() {
	if (is_empty()) return NULL;
	return _buffer + _front_i;
}

template <typename T, size_t SIZE>
T* SafeQueue<T, SIZE>::back() {
	if (is_empty()) return NULL;
	return _buffer + _get_prev_index(_end_i);
}

template <typename T, size_t SIZE>
T* SafeQueue<T, SIZE>::at(size_t i) {
	if (is_empty() || i >= size()) return NULL;
	return _buffer + ((_front_i + i) % SIZE);
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::front(T* data) {
	if (!SQ_GUARD_TRY_LOCK(_pop_guard)) return false;

	if (!data || is_empty()) {
		SQ_GUARD_UNLOCK(_pop_guard);
		return false;
	}
	memcpy((uint8_t *)data, (uint8_t *)(_buffer + _front_i), sizeof(T));

	SQ_GUARD_UNLOCK(_pop_guard);
	return true;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::back(T* data) {
	if (!SQ_GUARD_TRY_LOCK(_pop_guard)) return false;

	if (!data || is_empty()) {
		SQ_GUARD_UNLOCK(_pop_guard);
		return false;
	}
	memcpy((uint8_t *)data, (uint8_t *)(_buffer + _get_prev_index(_end_i)), sizeof(T));

	SQ_GUARD_UNLOCK(_pop_guard);
	return true;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::at(size_t i, T* data) {
	if (!SQ_GUARD_TRY_LOCK(_pop_guard)) return false;

	if (!data || is_empty() || i >= size()) {
		SQ_GUARD_UNLOCK(_pop_guard);
		return false;
	}
	memcpy((uint8_t *)data, (uint8_t *)(_buffer + ((_front_i + i) % SIZE)), sizeof(T));

	SQ_GUARD_UNLOCK(_pop_guard);
	return true;
}

template <typename T, size_t SIZE>
void SafeQueue<T, SIZE>::clear() {
	pop(size());
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::is_empty() {
	return _counter == 0x00;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::is_full() {
	return _counter == SIZE;
}

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::size() {
	return _counter;
}

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::available_space() {
	return SIZE - _counter;
}

template <typename T, size_t SIZE>
uint32_t SafeQueue<T, SIZE>::element_size() {
	return sizeof(T);
}


/* ------------- Private methods -------------*/
template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::_get_next_index(size_t base_index, size_t elements_cnt) {
	return (base_index + elements_cnt) % SIZE;
}

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::_get_prev_index(size_t base_index, size_t elements_cnt) {
	return (base_index >= elements_cnt)? (base_index - elements_cnt) : (SIZE - (elements_cnt - base_index));
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::_check_remove_ability(size_t elements_cnt) {
	return size() >= elements_cnt;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::_check_add_ability(size_t elements_cnt) {
	return (SIZE - size()) >= elements_cnt;
}

#endif /* SAFE_QUEUE_H_ */