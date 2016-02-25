#ifndef XPU_HASH_TABLE_H
#define XPU_HASH_TABLE_H

#include <functional>
#include <utility>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include <sys/types.h>

namespace xpu {
    namespace container {


        using namespace std;

        // define the standard hash functions.
        // based on the standard STL hash functions

        inline size_t __hash_string_func(const char* __s)
        {
            unsigned long __h = 0;
            for ( ; *__s; ++__s)
                __h = 5*__h + *__s;

            return size_t(__h);
        }


        template <class __key> struct __hash_t {
            size_t operator()(const __key & key) const { return key.hash(); }
        };

        template<> struct __hash_t<char*>
        {
        size_t operator()(const char* __s) const { return __hash_string_func(__s); }
    };

        template<> struct __hash_t<const char*>
        {
    size_t operator()(const char* __s) const { return __hash_string_func(__s); }
};
        template <> struct __hash_t<string> {
            size_t operator()(const string& __s) const { return __hash_string_func(__s.c_str()); }
        };

        template<> struct __hash_t<char> {
            size_t operator()(char __x) const { return __x; }
        };
        template<> struct __hash_t<unsigned char> {
            size_t operator()(unsigned char __x) const { return __x; }
        };
        template<> struct __hash_t<signed char> {
            size_t operator()(unsigned char __x) const { return __x; }
        };
        template<> struct __hash_t<short> {
            size_t operator()(short __x) const { return __x; }
        };
        template<> struct __hash_t<unsigned short> {
            size_t operator()(unsigned short __x) const { return __x; }
        };
        template<> struct __hash_t<int> {
            size_t operator()(int __x) const { return __x; }
        };
        template<> struct __hash_t<unsigned int> {
            size_t operator()(unsigned int __x) const { return __x; }
        };
        template<> struct __hash_t<long> {
            size_t operator()(long __x) const { return __x; }
        };
        template<> struct __hash_t<unsigned long> {
            size_t operator()(unsigned long __x) const { return __x; }
        };

        /**
   * @}
   */

        // the number of buckets should be a prime number
        // to spread elements out nicely, we also want to
        // double the size of the hash map each time we resize.
        // hence we need this list of primes.
        static const int __hash_table_prime_count = 28;
        static const unsigned long __hashTablePrimes[__hash_table_prime_count] =
        {
            53ul,         97ul,         193ul,       389ul,       769ul,
            1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
            49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
            1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
            50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
            1610612741ul, 3221225473ul, 4294967291ul
        };


        // get the next prime number from our primes array, equal to or
        // greater than n.
        static unsigned long __get_next_hash_table_prime(unsigned long n) {
            const unsigned long* first = __hashTablePrimes;
            const unsigned long* last = __hashTablePrimes + __hash_table_prime_count;
            const unsigned long* pos = lower_bound(first, last, n);
            return pos == last ? *(last - 1) : *pos;
        }


        /**
   * This is an implementation of a hash table for use with HashMap and
   * HashSet, which in turn are replacements for hash_map and hash_set,
   * which for some bizarre reason are not part of the C++ standard and
   * hence not supported by some compilers.
   */
        template <class __key, class __value, class __key_extractor, class __hasher, class __equalizer>
                class hash_table {

            // Implementation details: we store a vector of buckets, each of
            // which contains a singly linked list of Node objects, which hold
            // the hash table data. This implementation is very similar to that
            // provided in the original STL from SGI and HP. The hashtable is
            // always sized to make the number of buckets a prime number. This
            // reduces the possibility of poor hash functions skipping cells in
            // the table (e.g. imagine a hash function that produces a number
            // that's always a multiple of 5, combined with a table size that
            // has 5 as a factor). Once the average number of elements in each
            // cell begins to rise above a certain fraction, we need to resize
            // the table in order to maintain efficiency. In this
            // implementation, that fraction is 1.0, i.e. the number of elements
            // is always less than or equal to the number of buckets. When we
            // resize we need to increase the table size by a constant factor in
            // order to maintain amortized constant time insertion. So we go to
            // the next prime that is approximately twice as large as the
            // current size.

                public:

            // Node is the basic entity used for storing values
            struct node_t {

                __value value;
                node_t* next;

                node_t(const __value& v, node_t* n = 0) : value(v), next(n) { }

            };

                private:

            // The hash table consists of a vector of pointers to lists of Nodes
            vector<node_t*> buckets;

            __hasher hasher;
            __equalizer equalizer;
            __key_extractor key_extractor;
            size_t num_elements;

            size_t bucket_index(const __value& value) const {
                return hasher(key_extractor(value)) % buckets.size();
            }

            size_t bucket_index(const __value& value, size_t n) const {
                return hasher(key_extractor(value)) % n;
            }

            size_t bucket_index_key(const __key& key) const {
                return hasher(key) % buckets.size();
            }

                public:

            typedef hash_table<__key, __value, __key_extractor, __hasher, __equalizer> hash_table_t;

            // Iterator definitions

            struct iterator {

                node_t* node;
                hash_table_t* h_table;

                iterator() { }

                iterator(node_t* n, hash_table_t* h) : node(n), h_table(h) { }

                __value& operator*() const {
                    return node->value;
                }

                iterator& operator++();

                iterator& operator++(int);

                bool operator==(const iterator& it) const {
                    return node == it.node;
                }

                bool operator!=(const iterator& it) const {
                    return node != it.node;
                }

            };

            friend struct iterator;

            struct const_iterator {

                const node_t* node;
                const hash_table_t* hashTable;

                const_iterator() { }

                const_iterator(const node_t* n, const hash_table_t* h) : node(n), hashTable(h) { }

                const __value& operator*() const {
                    return node->value;
                }

                const_iterator& operator++();

                const_iterator& operator++(int);

                bool operator==(const const_iterator& it) const {
                    return node == it.node;
                }

                bool operator!=(const const_iterator& it) const {
                    return node != it.node;
                }

            };

            friend struct const_iterator;

            // Constructors and Destructors

            hash_table(size_t n = 0) {
                initialize(n);
            }

            hash_table(size_t n, const __hasher& hasher_) : hasher(hasher_) {
                initialize(n);
            }

            hash_table(size_t n, const __hasher& hasher_, const __equalizer& equalizer_) : hasher(hasher_), equalizer(equalizer_) {
                initialize(n);
            }

            hash_table(const hash_table_t& ht) {
                initialize(ht.size());
                for (typename hash_table_t::const_iterator i = ht.begin(); i != ht.end(); ++i)
                    insert(*i);
            }

            hash_table_t& operator=(const hash_table_t& ht) {
                if (&ht != this) {
                    clear();
                    initialize(ht.size());
                    for (typename hash_table_t::const_iterator i = ht.begin(); i != ht.end(); ++i)
                        insert(*i);
                }
                return *this;
            }

            ~hash_table() {
                clear();
            }

            // Member functions

            iterator end() {
                return iterator(0, this);
            }

            iterator begin() {
                for (size_t i = 0; i < buckets.size(); ++i) {
                    if (buckets[i])
                        return iterator(buckets[i], this);
                }
                return end();
            }

            const_iterator end() const {
                return const_iterator(0, this);
            }

            const_iterator begin() const {
                for (size_t i = 0; i < buckets.size(); ++i) {
                    if (buckets[i])
                        return const_iterator(buckets[i], this);
                }
                return end();
            }

            size_t size() const {
                return num_elements;
            }

            size_t max_size() const {
                return buckets.size();
            }

            size_t bucket_count() const {
                return buckets.size();
            }

            bool empty() const {
                return num_elements == 0;
            }

            __hasher hash_funct() const {
                return hasher;
            }

            __equalizer key_eq() const {
                return equalizer;
            }

            void resize(size_t n) {    // resize only if bucket count is less than required element count
                size_t old_size = buckets.size();
                if (n > old_size) {
                    size_t newSize = __get_next_hash_table_prime(n);
                    // Create a new temporary hash table
                    vector<node_t*> tmp(newSize, 0);
                    for (size_t b = 0; b < old_size; ++b) {
                        node_t* node = buckets[b];
                        while (node) {
                            size_t index = bucket_index(node->value, newSize);
                            buckets[b] = node->next;
                            node->next = tmp[index];
                            tmp[index] = node;
                            node = buckets[b];
                        }
                    }
                    // Now we need to copy the temporary table back into the old one
                    buckets.resize(newSize);
                    for (size_t b = 0; b < newSize; ++b)
                        buckets[b] = tmp[b];
                }
            }

            pair<iterator,bool> insert(const __value& value) {
                resize(num_elements + 1);
                __key key = key_extractor(value);
                size_t index = bucket_index_key(key);
                node_t* node = buckets[index];
                while (node) {
                    if (equalizer(key, key_extractor(node->value)))
                        return make_pair(iterator(node, this), false);
                    node = node->next;
                }
                // If we get here the key was not found
                node_t* newNode = new node_t(value, buckets[index]);
                ++num_elements;
                buckets[index] = newNode;
                return make_pair(iterator(newNode, this), true);
            }

            void clear() {
                for (size_t b = 0; b < buckets.size(); ++b) {
                    node_t* node = buckets[b];
                    while (node) {
                        buckets[b] = node->next;
                        delete node;
                        node = buckets[b];
                    }
                }
                num_elements = 0;
            }

            iterator find(const __key& key) {
                size_t index = bucket_index_key(key);
                node_t* node = buckets[index];
                while (node) {
                    if (equalizer(key, key_extractor(node->value)))
                        return iterator(node, this);
                    node = node->next;
                }
                return end();
            }

            const_iterator find(const __key& key) const {
                size_t index = bucket_index_key(key);
                const node_t* node = buckets[index];
                while (node) {
                    if (equalizer(key, key_extractor(node->value)))
                        return const_iterator(node, this);
                    node = node->next;
                }
                return end();
            }

            size_t count(const __key& key) const {
                return find(key) == end() ? 0 : 1;
            }

            void erase(iterator pos) {
                node_t* target_node = pos.node;
                if (target_node) {
                    size_t index = bucket_index(target_node->value);
                    node_t* node = buckets[index];

                    if (node == target_node) {
                        buckets[index] = node->next;
                        delete node;
                        --num_elements;
                    }
                    else {
                        node_t* next_node = node->next;
                        while (next_node) {
                            if (next_node == target_node) {
                                node->next = next_node->next;
                                delete next_node;
                                --num_elements;
                                break;
                            }
                            else {
                                node = next_node;
                                next_node = node->next;
                            }
                        }
                    }
                }
            }

            // Note that this version assumes unique keys and so will only delete one item
            size_t erase(const __key& key) {
                size_t index = bucket_index_key(key);
                node_t* node = buckets[index];
                size_t erased = 0;

                // If bucket is empty then we're not going to erase anything
                if (!node)
                    return 0;

                if (equalizer(key_extractor(node->value), key)) {
                    buckets[index] = node->next;
                    delete node;
                    --num_elements;
                    ++erased;
                }
                else {
                    node_t* next_node = node->next;
                    while (next_node) {
                        if (equalizer(key_extractor(next_node->value), key)) {
                            node->next = next_node->next;
                            delete next_node;
                            --num_elements;
                            ++erased;
                            break;
                        }
                        else {
                            node = next_node;
                            next_node = node->next;
                        }
                    }
                }
                return erased;
            }

        private:

            void initialize(size_t n) {
                num_elements = 0;
                size_t num_buckets = __get_next_hash_table_prime(n);
                buckets.resize(num_buckets, 0);
            }

        };


        template <class __key, class __value, class __key_extractor, class __hasher, class __equalizer>
                typename hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::iterator&
                hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::iterator::operator++() {
            typename hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::node_t* old = node;
            node = node->next;
            if (!node) {
                size_t bucket = h_table->bucket_index(old->value);
                while (!node && ++bucket < h_table->buckets.size())
                    node = h_table->buckets[bucket];
            }
            return *this;
        }

        template <class __key, class __value, class __key_extractor, class __hasher, class __equalizer>
                typename hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::iterator&
                hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::iterator::operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        template <class __key, class __value, class __key_extractor, class __hasher, class __equalizer>
                typename hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::const_iterator&
                hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::const_iterator::operator++() {
            typename hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::node_t const* old = node;
            node = node->next;
            if (!node) {
                size_t bucket = hashTable->bucket_index(old->value);
                while (!node && ++bucket < hashTable->buckets.size())
                    node = hashTable->buckets[bucket];
            }
            return *this;
        }

        template <class __key, class __value, class __key_extractor, class __hasher, class __equalizer>
                typename hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::const_iterator&
                hash_table<__key, __value, __key_extractor, __hasher, __equalizer>::const_iterator::operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

    } //container
} // namespace xpu

#endif







