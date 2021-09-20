// Mazen Ibrahim
//295924
#ifndef RING_H
#define RING_H

#include <iterator>
#include <iostream>

template <typename Key, typename Info>
class Ring {
public:
    class ring_node sealed {
    public:
        ring_node(void) : key(), info(), next(nullptr), prev(nullptr) { }

        ring_node(Key k, Info i, ring_node* n = nullptr, ring_node* p = nullptr)
            : key(k), info(i), next(n), prev(p) { }

        const Key& get_key(void) const {
            return key;
        }

        const Info& get_info(void) const {
            return info;
        }

        void set_info(const Info& value) {
            info = value;
        }

    private:
        Key key;
        Info info;
        ring_node* next;
        ring_node* prev;

        friend class Ring<Key, Info>;
        friend class Ring<Key, Info>::iterator;
    };

    class iterator : public std::iterator<std::bidirectional_iterator_tag, Key> {
    public:
        iterator(void) : node(nullptr) { }
        iterator(ring_node* n = nullptr) : node(n) { }
        iterator(const iterator& copy) : node(copy.node) { }

        ring_node* operator *(void) const { return this->node; }
        ring_node* operator ->(void) const { return this->node; }

        bool operator ==(const iterator& iter) { return this->node == iter.node; }
        bool operator !=(const iterator& iter) { return this->node != iter.node; }

        iterator& operator ++(void) { node = (node == nullptr ? nullptr : node->next); return *this; }
        iterator& operator --(void) { node = (node == nullptr ? nullptr : node->prev); return *this; }

        iterator& operator =(const iterator& iter) {
            if (this != &iter)
                node = iter.node;
            return *this;
        }

        iterator operator ++(int) {
            iterator prev = *this;
            ++(*this);
            return prev;
        }

        iterator operator --(int) {
            iterator prev = *this;
            --(*this);
            return prev;
        }

    protected:
        ring_node* node;
        friend class Ring<Key, Info>;
    };

    Ring(void) : seam(nullptr) { }
    Ring(const Ring& copy) : seam(nullptr) {
        copyFrom(copy);
    }
    ~Ring(void) { clear(); }

    Ring& operator =(const Ring& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    void clear(void) {
        while (any() != none()) {
            auto iter = any();
            remove(iter);
        }
    }

    void insert(const Key& key, const Info& info, bool direction = true) {
        if (seam == nullptr) {
            seam = new ring_node(key, info, nullptr, nullptr);
            seam->prev = seam->next = seam;
        }
        else {
            ring_node* node = new ring_node(key, info, nullptr, nullptr);
            ring_node* next = nullptr;
            ring_node* prev = nullptr;
            if (direction) {
                next = seam;
                prev = seam->prev;
            }
            else {
                prev = seam;
                next = seam->next;
            }
            node->prev = prev;
            node->next = next;
            prev->next = node;
            next->prev = node;
            seam = node;
        }
    }

    void insert(iterator& at, const Key& key, const Info& info) {
        ring_node* node = at.node;
        ring_node* next = node;
        ring_node* prev = node->prev;
        if (at == seam) {
            insert(key, info);
            at.node = seam;
        }
        else {
            node = new ring_node(key, info, next, prev);
            prev->next = node;
            next->prev = node;
            at.node = node;
        }
    }

    void insert(iterator& at, const Key& key, const Info& info, int count) {
        for (; count > 0; --count) insert(at, key, info);
    }

    void remove(iterator& at, bool direction = true) {
        ring_node* node = at.node;
        if (node != nullptr) {
            ring_node* next = node->next;
            ring_node* prev = node->prev;
            next->prev = prev;
            prev->next = next;
            if (node == seam) {
                if (direction) {
                    seam = next;
                }
                else {
                    seam = prev;
                }
                if (node == next && node == prev) {
                    next = prev = seam = at.node = nullptr;
                }
            }
            if (direction) {
                at.node = next;
            }
            else {
                at.node = prev;
            }
            delete node;
        }
    }

    void remove(iterator& at, int count) {
        for (; count > 0; --count) remove(at);
    }

    void remove(iterator& from, iterator& to) {
        do {
            remove(from);
        } while (from != to);
        remove(to);
    }

    bool find(const Key& key, iterator& iter, bool direction) {
        iterator first = iter;
        do {
            if ((iter++)->get_key() == key) {
                return true;
            }
        } while (iter != first);
        return false;
    }

    bool find(const Key& key, int occurrence, iterator& iter, bool direction) {
        iterator check = iter;
        int state = occurrence;
        if (direction) {
            do {
                if ((iter++)->get_key() == key) {
                    --occurrence;
                }
                if (check == iter && state == occurrence) {
                    return false;
                }
            } while (occurrence != 0);
        }
        else {
            do {
                if ((iter--)->get_key() == key) {
                    --occurrence;
                }
                if (check == iter && state == occurrence) {
                    return false;
                }
            } while (occurrence != 0);
        }
        --iter;
        return true;
    }

    iterator any(void) const { return iterator(seam); }
    iterator none(void) const { return iterator(nullptr); }

    friend inline std::ostream& operator <<(std::ostream& out, Ring<Key, Info> ring) {
        iterator iter = ring.any();
        if (iter != ring.none()) {
            do {
                out << "[" << iter->get_key() << ", " << iter->get_info() << "]" << std::endl;
            } while (++iter != ring.any());
        }
        return out;
    }

private:
    void copyFrom(const Ring& from) {
        if (from.seam != nullptr) {
            ring_node* src = from.seam;
            ring_node* trg = seam;

            do {
                ring_node* node = new ring_node(src->key, src->info, nullptr, nullptr);
                if (trg == nullptr) {
                    seam = trg = node;
                    node->next = node;
                    node->prev = node;
                }
                else {
                    trg->next = node;
                    node->prev = trg;
                }
                trg = node;
                src = src->next;
            } while (src != from.seam);

            seam->prev = trg;
            trg->next = seam;
        }
    }

    ring_node* seam;
};
#endif