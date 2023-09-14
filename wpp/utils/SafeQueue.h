// TODO: Add lable taht this code belongs SinaiR&D
#ifndef INC_LITE_QUEUE_H_
#define INC_LITE_QUEUE_H_

#include <string.h>
#include <vector>
#include <mutex>

#define LITE_QUEUE_DEF_ELEM_CNT	0x01

/*
 * This class is a simple implementation of a queue that does not use dynamic memory
 * and copy data to intenal buffer with memcpy.
 * The problem it solves is the following: it is necessary to have a container that
 * allows adding and removing elements independently by different streams. This
 * container is protected from the simultaneous call of the push/pop method from
 * different threads, and allows the simultaneous call of push and pop from different
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
	SafeQueue() {
		__push_guard.unlock();
		__pop_guard.unlock();
	}

	bool push(const T * const data, size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT);
	bool pop(size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT);

	// TODO Add ability converting vector to SafeQueue 'from_vector(std::vector<T>)'
	std::vector<T> to_vector(size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT);

	/*
	 * front/back/at(index) They do not guarantee that the pointer
	 * to the data after the return will still be correct
	 */
	T* front();
	T* back();
	T* at(size_t i);
	/*
	 * Can guarantee the integrity of the data and ensures that it
	 * corresponds to what was written to the queue. The user needs
	 * to pass a pointer to the memory allocated for this type.
	 */
	bool front(T* data);
	bool back(T* data);
	bool at(size_t i, T* data);

	bool is_empty();
	bool is_full();
	size_t size();
	size_t empty_size();
	uint32_t element_size();

private:
	/**
	 * Before calling these method, validate items count in the queue
	 */
	inline size_t __get_next_front_index(size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT);
	inline size_t __get_next_back_index(size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT);
	inline size_t __get_next_index(size_t base_index, size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT, uint8_t is_offset = false);
	inline size_t __get_prev_index(size_t base_index, size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT);

	inline bool __check_remove_ability(size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT);
	inline bool __check_add_ability(size_t elements_cnt = LITE_QUEUE_DEF_ELEM_CNT);

private:
	std::mutex __push_guard;
	std::mutex __pop_guard;
	size_t __front_i = 0x00, __back_i = 0x00, __counter = 0x00;
	T __buffer[SIZE];
};


/* ------------- Public methods -------------*/

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::push(const T * const data, size_t elements_cnt) {
	if (!__push_guard.try_lock()) return false;

	/* Check __buffer available space */
	if (!elements_cnt || __check_add_ability(elements_cnt) == false) {
		__push_guard.unlock();
		return false;
	}

	for (size_t i = 0; i < elements_cnt; i++) {
		__back_i = __get_next_back_index();
		memcpy((uint8_t *)(__buffer + __back_i), (uint8_t *)(data + i), sizeof(T));
	}
	__counter += elements_cnt;

	__push_guard.unlock();
	return true;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::pop(size_t elements_cnt) {
	if (!__pop_guard.try_lock()) return false;

	/* Maybe the __buffer available elements not enough */
	if (__check_remove_ability(elements_cnt) == false) { 
		__pop_guard.unlock();
		return false;
	}

	__front_i = __get_next_front_index(elements_cnt);
	__counter -= elements_cnt;

	__pop_guard.unlock();
	return true;
}

template <typename T, size_t SIZE>
std::vector<T> SafeQueue<T, SIZE>::to_vector(size_t elements_cnt) {
	std::lock_guard<std::mutex> push_guard(__push_guard);
	std::lock_guard<std::mutex> pop_guard(__pop_guard);

	/* Maybe the __buffer available elements not enough */
	if (!elements_cnt || elements_cnt > size()) return std::vector<T>();

	size_t end_of_range_index = __get_next_index(__front_i, elements_cnt);
	uint8_t is_out_of_range = __front_i >= end_of_range_index;

	/*
	 * Available two potential situations:
	 * 1) Elements range placed between __front_i and SIZE
	 * 2) Elements range placed between __front_i -> SIZE and 0 -> end_of_range_index
	 *
	 * If is_out_of_range set to true then we have second situation,
	 * and converting operation will be performed in two steps
	 */
	std::vector<T> converted (__buffer + __front_i, (is_out_of_range)? __buffer + SIZE : __buffer + end_of_range_index);
	if (is_out_of_range) converted.insert(converted.end(), __buffer, __buffer + end_of_range_index);

	return converted;
}

template <typename T, size_t SIZE>
T* SafeQueue<T, SIZE>::front() {
	if (is_empty()) return NULL;
	return __buffer + __front_i;
}

template <typename T, size_t SIZE>
T* SafeQueue<T, SIZE>::back() {
	if (is_empty()) return NULL;
	return __buffer + __back_i;
}

template <typename T, size_t SIZE>
T* SafeQueue<T, SIZE>::at(size_t i) {
	if (is_empty() || i >= size()) return NULL;
	return __buffer + ((__front_i + i) % SIZE);
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::front(T* data) {
	if (!__pop_guard.try_lock()) return false;

	if (!data || is_empty()) {
		__pop_guard.unlock();
		return false;
	}
	memcpy((uint8_t *)data, (uint8_t *)(__buffer + __front_i), sizeof(T));

	__pop_guard.unlock();
	return true;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::back(T* data) {
	if (!__push_guard.try_lock()) return false;

	if (!data || is_empty()) {
		__push_guard.unlock();
		return false;
	}
	memcpy((uint8_t *)data, (uint8_t *)(__buffer + __back_i), sizeof(T));

	__push_guard.unlock();
	return true;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::at(size_t i, T* data) {
	if (!__pop_guard.try_lock()) return false;

	if (!data || is_empty() || i >= size()) {
		__pop_guard.unlock();
		return false;
	}
	memcpy((uint8_t *)data, (uint8_t *)(__buffer + ((__front_i + i) % SIZE)), sizeof(T));

	__pop_guard.unlock();
	return true;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::is_empty() {
	return __counter == 0x00;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::is_full() {
	return __counter == SIZE;
}

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::size() {
	return __counter;
}

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::empty_size() {
	return SIZE - __counter;
}

template <typename T, size_t SIZE>
uint32_t SafeQueue<T, SIZE>::element_size() {
	return sizeof(T);
}


/* ------------- Private methods -------------*/

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::__get_next_front_index(size_t elements_cnt) {
	// When __buffer size equals to elements_cnt needs to do offset by one element
	return __get_next_index(__front_i, elements_cnt, size() == elements_cnt);
}

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::__get_next_back_index(size_t elements_cnt) {
	// When __buffer is empty __front_i and __back_i are equal therefore needs to do offset by one element
	return __get_next_index(__back_i, elements_cnt, is_empty());
}

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::__get_next_index(size_t base_index, size_t elements_cnt, uint8_t is_offset) {
	size_t next_index = base_index + elements_cnt - ((is_offset && elements_cnt)? 1 : 0);
	return next_index % SIZE;
}

template <typename T, size_t SIZE>
size_t SafeQueue<T, SIZE>::__get_prev_index(size_t base_index, size_t elements_cnt) {
	return (base_index >= elements_cnt)? base_index - elements_cnt : SIZE - (elements_cnt - base_index);
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::__check_remove_ability(size_t elements_cnt) {
	return size() >= elements_cnt;
}

template <typename T, size_t SIZE>
bool SafeQueue<T, SIZE>::__check_add_ability(size_t elements_cnt) {
	return (SIZE - size()) >= elements_cnt;
}

#endif /* INC_LITE_QUEUE_H_ */
