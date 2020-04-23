#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {
    const int SIZE = 250;

template<class T>
class deque {

public:
    class block{
    public:
        T **arr;
        int len;
        block *pre, *nxt;

        block(block *_pre = NULL, block *_nxt = NULL) : pre(_pre), nxt(_nxt){
            len = 0;
            arr = new T*[SIZE];
            for(int i = 0;i < SIZE;++i) arr[i] = NULL;
        }
        ~block(){
            for(int i = 0;i < SIZE;++i) if(arr[i]) delete arr[i];
            delete[] arr;
        }
    };

	class const_iterator;
	class iterator {
	private:
	    deque *it_bel;
	    block *it_b;
	    int pos;

	public:

	    iterator(deque *_bel = NULL, block *_b = NULL, int _pos = 0) : it_bel(_bel), it_b(_b), pos(_pos){}

		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
		    if(n < 0) return (*this) - (-n);

		    iterator res = (*this);
		    int lef = n;
		    while(lef + res.pos >= res.it_b->len){
		        lef -= res.it_b->len - res.pos;
		        res.it_b = res.it_b->nxt;
		        res.pos = 0;
		    }
		    res.pos += lef;
		    return res;
		}
		iterator operator-(const int &n) const {
            if(n < 0) return (*this) + (-n);

            iterator res = (*this);
            int lef = n;
            while(lef > res.pos){
                lef -= res.pos + 1;
                res.it_b = res.it_b->pre;
                res.pos = res.it_b->len - 1;
            }
            res.pos -= lef;
            return res;
		}

		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
            if(it_bel != rhs.it_bel) throw invalid_iterator();
            if(it_b == rhs.it_b) return pos - rhs.pos;

            int res = it_b->len - pos + rhs.pos;
            for(block *t = it_b->nxt; t; t = t->nxt){
                if(t == rhs.it_b) return -res;
                res += t->len;
            }

            res = rhs.it_b->len - rhs.pos + pos;
            for(block *t = rhs.it_b->nxt; t; t = t->nxt){
                if(t == it_b) return res;
                res += t->len;
            }
            throw invalid_iterator();
		}

		iterator& operator+=(const int &n) {
			(*this) = (*this) + n;
			return (*this);
		}
		iterator& operator-=(const int &n) {
            (*this) = (*this) - n;
            return (*this);
		}

		//TODO iter++
		iterator operator++(int) {
		    if((*this) == it_bel->end()) throw invalid_iterator();
            iterator res = (*this);
            (*this) = (*this) + 1;
            return res;
		}

		//TODO ++iter
		iterator& operator++() {
		    if((*this) == it_bel->end()) throw invalid_iterator();
            (*this) = (*this) + 1;
            return (*this);
		}

		//TODO iter--
		iterator operator--(int) {
		    if((*this) == it_bel->begin()) throw invalid_iterator();
            iterator res = (*this);
		    (*this) = (*this) - 1;
            return res;
		}

		//TODO --iter
		iterator& operator--() {
            if((*this) == it_bel->begin()) throw invalid_iterator();
            (*this) = (*this) - 1;
            return (*this);
		}

		//*it
		T& operator*() const {
		    if((*this) == it_bel->end()) throw invalid_iterator();
		    return *(it_b->arr[pos]);
		}

		//it->field
		T* operator->() const noexcept {
		    return (it_b->arr[pos]);
		}

		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {return it_b == rhs.it_b && pos == rhs.pos;}
		bool operator==(const const_iterator &rhs) const {return it_b == rhs.it_b && pos == rhs.pos;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return !((*this) == rhs);}
		bool operator!=(const const_iterator &rhs) const {return !((*this) == rhs);}
        friend class const_iterator;
        friend class deque<T>;
	};

	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.

	private:
            deque *it_bel;
            block *it_b;
            int pos;
	public:
		const_iterator(deque *_bel = NULL, block *_b = NULL, int _pos = 0) : it_bel(_bel), it_b(_b), pos(_pos){}

        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is **undefined**.
         * as well as operator-
         */
        const_iterator operator+(const int &n) const {
            if(n < 0) return (*this) - (-n);

            const_iterator res = (*this);
            int lef = n;
            while(lef + res.pos >= res.it_b->len){
                lef -= res.it_b->len - res.pos;
                res.it_b = res.it_b->nxt;
                res.pos = 0;
            }
            res.pos += lef;
            return res;
        }
        const_iterator operator-(const int &n) const {
            if(n < 0) return (*this) + (-n);

            const_iterator res = (*this);
            int lef = n;
            while(lef > res.pos){
                lef -= res.pos + 1;
                res.it_b = res.it_b->pre;
                res.pos = res.it_b->len - 1;
            }
            res.pos -= lef;
            return res;
        }

        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            if(it_bel != rhs.it_bel) throw invalid_iterator();
            if(it_b == rhs.it_b) return pos - rhs.pos;

            int res = it_b->len - pos + rhs.pos;
            for(block *t = it_b->nxt; t; t = t->nxt){
                if(t == rhs.it_b) return -res;
                res += t->len;
            }

            res = rhs.it_b->len - rhs.pos + pos;
            for(block *t = rhs.it_b->nxt; t; t = t->nxt){
                if(t == it_b) return res;
                res += t->len;
            }
            throw invalid_iterator();
        }

        const_iterator& operator+=(const int &n) {
            (*this) = (*this) + n;
            return (*this);
        }
        const_iterator& operator-=(const int &n) {
            (*this) = (*this) - n;
            return (*this);
        }

        //TODO iter++
        const_iterator operator++(int) {
            if((*this) == it_bel->cend()) throw invalid_iterator();
            const_iterator res = (*this);
            (*this) = (*this) + 1;
            return res;
        }

        //TODO ++iter
        const_iterator& operator++() {
            if((*this) == it_bel->cend()) throw invalid_iterator();
            (*this) = (*this) + 1;
            return (*this);
        }

        //TODO iter--
        const_iterator operator--(int) {
            if((*this) == it_bel->cbegin()) throw invalid_iterator();
            const_iterator res = (*this);
            (*this) = (*this) - 1;
            return res;
        }

        //TODO --iter
        const_iterator& operator--() {
            if((*this) == it_bel->cbegin()) throw invalid_iterator();
            (*this) = (*this) - 1;
            return (*this);
        }

        //*it
        T& operator*() const {
            if((*this) == it_bel->cend()) throw invalid_iterator();
            return *(it_b->arr[pos]);
        }

        //it->field
        T* operator->() const noexcept {
            return (it_b->arr[pos]);
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {return it_b == rhs.it_b && pos == rhs.pos;}
        bool operator==(const const_iterator &rhs) const {return it_b == rhs.it_b && pos == rhs.pos;}
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {return !((*this) == rhs);}
        bool operator!=(const const_iterator &rhs) const {return !((*this) == rhs);}

        friend class iterator;
        friend class deque<T>;
	};

private:
    int tot, head_pos;// head_pos: may delete from head
    block *head, *tail;

public:
    /**
	 * TODO Constructors
	 */
	deque() {
	    tot = 0;
        head = new block();
        tail = head;
        head->len = 1; head_pos = 0;
	}

	deque(const deque &other) {
	    tot = other.tot;
	    head = new block();
	    for(int i = 0;i < SIZE;++i)
	        if(other.head->arr[i]) head->arr[i] = new T(*other.head->arr[i]);

	    head->len = other.head->len;
	    head_pos = other.head_pos;
	    if(other.head == other.tail) tail = head;

	    block *before = head, *now;
	    for(block *t = other.head->nxt;t;t = t->nxt){
	        now = new block();
	        for(int i = 0;i < SIZE;++i)
	            if(t->arr[i]) now->arr[i] = new T(*t->arr[i]);
	        now->len = t->len;

	        before->nxt = now;
	        now->pre = before;
	        if(other.tail == t) tail = now;
	        before = now;
	    }
	}

	/**
	 * TODO Deconstructor
	 */
	~deque() {
	    block *tmp;
        for(block *t = head->pre;t;){
            tmp = t; t = t->pre;
            delete tmp;
        }
        for(block *t = head;t;){
	        tmp = t; t = t->nxt;
	        delete tmp;
	    }
    }

	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
	    if((&other) == this) return (*this);
	    this->~deque();

        tot = other.tot;
        head = new block();
        for(int i = 0;i < SIZE;++i)
            if (other.head->arr[i]) head->arr[i] = new T(*other.head->arr[i]);

        head->len = other.head->len;
        head_pos = other.head_pos;
        if(other.head == other.tail) tail = head;

        block *before = head, *now;
        for(block *t = other.head->nxt;t;t = t->nxt){
            now = new block();
            for(int i = 0;i < SIZE;++i)
                if(t->arr[i]) now->arr[i] = new T(*t->arr[i]);

            now->len = t->len;

            before->nxt = now;
            now->pre = before;
            if(other.tail == t) tail = now;

            before = now;
        }
        return (*this);
    }

	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
	    if(pos < 0 || pos >= tot) throw index_out_of_bound();

	    if(head == tail || head_pos + pos < head->len)
	        return *(head->arr[head_pos+pos]);
	    else{
	        int lef = pos - (head->len - head_pos);
	        for(block *t = head->nxt;t;t = t->nxt){
	            if(lef < t->len) return *(t->arr[lef]);
	            lef -= t->len;
	        }
	    }
	    throw index_out_of_bound();
	}
	const T & at(const size_t &pos) const {
        if(pos < 0 || pos >= tot) throw index_out_of_bound();

        if(head == tail || head_pos + pos < head->len)
            return *(head->arr[head_pos+pos]);
        else{
            int lef = pos - (head->len - head_pos);
            for(block *t = head->nxt;t;t = t->nxt){
                if(lef < t->len) return *(t->arr[lef]);
                lef -= t->len;
            }
        }
        throw index_out_of_bound();
    }

	T & operator[](const size_t &pos) {return at(pos);}
	const T & operator[](const size_t &pos) const {return at(pos);}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
	    if(!tot) throw container_is_empty();
	    return *(head->arr[head_pos]);
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
	    if(!tot) throw container_is_empty();
	    return at(tot-1);
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {return iterator(this,head,head_pos);}
	const_iterator cbegin() const {return const_iterator(const_cast<deque*>(this),head,head_pos);}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {return iterator(this,tail,tail->len-1);}
	const_iterator cend() const {return const_iterator(const_cast<deque*>(this),tail,tail->len-1);}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {return tot == 0;}
	/**
	 * returns the number of elements
	 */
	size_t size() const {return tot;}
	/**
	 * clears the contents
	 */
	void clear() {
	    this->~deque();
        tot = 0;
        head = new block();
        tail = head;
        head->len = 1; head_pos = 0;
	}

	//combine t with t_nxt if the sum of size < SIZE

	void combine(block *t){
	    if(!t || !t->nxt || t == head || t->nxt == head || t == tail ||
	      t->nxt == tail || t->len + t->nxt->len > SIZE) return;

        block *tmp = t->nxt;
	    for(int i = 0, j = t->len;i < tmp->len;++i, ++j){
	        t->arr[j] = tmp->arr[i];
	        tmp->arr[i] = NULL;
	    }

	    t->len += tmp->len;
	    if(tmp->nxt) tmp->nxt->pre = t;
	    t->nxt = tmp->nxt;
	    delete tmp;
	}

	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator in, const T &value, bool check = true) {
        if(head == in.it_b && in.pos < head_pos) throw invalid_iterator();

        if(head == in.it_b && in.pos == head_pos){
            ++tot;
            if(head_pos){
                head_pos--;
                if(head->arr[head_pos]) delete head->arr[head_pos];
                head->arr[head_pos] = new T(value);
            }
            else{
                block *t;
                if(head->pre) t = head->pre;
                else {
                    t = new block(NULL, head);
                    head->pre = t;
                }

                if(t->arr[SIZE-1]) delete t->arr[SIZE-1];
                t->arr[SIZE-1] = new T(value);
                head = t; head_pos = SIZE-1; head->len = SIZE;
            }
            return begin();
        }
        else{
            //check whether the iterator is invalid
            if(check) {
                bool F = 0;
                if (in.it_b == head) F = 1;
                else {
                    for (block *t = head->nxt; t; t = t->nxt) {
                        if (t == in.it_b && in.pos < t->len) {
                            F = 1;
                            break;
                        }
                    }
                }
                if (!F) throw invalid_iterator();
            }

            ++tot;
            if(in.it_b->len == SIZE){
                block *t;
                if(in.it_b == tail){
                    if(tail->nxt) t = tail->nxt;
                    else t = new block(tail, NULL);
                }
                else
                    t = new block(in.it_b, in.it_b->nxt);

                t->arr[0] = in.it_b->arr[SIZE-1];
                t->len = 1;

                if(in.it_b == tail) tail = t;
                t->pre->nxt = t;
                if(t->nxt) t->nxt->pre = t;

                --in.it_b->len;
                combine(t);
            }

            ++in.it_b->len;

            for(int i = in.it_b->len-1;i > in.pos;--i)
                in.it_b->arr[i] = in.it_b->arr[i-1];
            in.it_b->arr[in.pos] = new T(value);
            return in;
        }
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator in, bool check = true) {
	    if(!tot || (head == in.it_b && in.pos < head_pos)) throw invalid_iterator();
        if(head == in.it_b && in.pos == head_pos){
            --tot;
            if(head->arr[head_pos]) delete head->arr[head_pos];
            head->arr[head_pos] = NULL;

            if(head_pos + 1 < head->len) head_pos++;
            else {head = head->nxt; head_pos = 0;}
            return iterator(this, head, head_pos);
        }
        else{
            //check whether the iterator is invalid
            if(check){
                bool F = 0;
                if(in.it_b == head) F = 1;
                else{
                    for(block *t = head->nxt;t;t = t->nxt) {
                        if (t == in.it_b && in.pos < t->len) {
                            F = 1;
                            break;
                        }
                    }
                }
                if(!F) throw invalid_iterator();
            }

            --tot;
            if(in.it_b->len == 1){
                block *t = in.it_b->pre;
                t->nxt = in.it_b->nxt;
                if(t->nxt) t->nxt->pre = t;
                delete in.it_b;
                return iterator(this, t->nxt, 0);
            }

            if(in.it_b->arr[in.pos]) delete in.it_b->arr[in.pos];
            in.it_b->arr[in.pos] = NULL;

            --in.it_b->len;
            for(int i = in.pos;i < in.it_b->len;++i)
                in.it_b->arr[i] = in.it_b->arr[i+1];
            in.it_b->arr[in.it_b->len] = NULL;

            combine(in.it_b);
            if (in.pos >= in.it_b->len) {
                in.it_b = in.it_b->nxt;
                in.pos = 0;
            }

            return in;
        }
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {insert(end(),value,0);}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
	    if(!tot) throw container_is_empty();
	    erase(end()-1,0);
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) { insert(begin(), value, 0);}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
	    if(!tot) throw container_is_empty();
	    erase(begin(),0);
	}
};

}

#endif
