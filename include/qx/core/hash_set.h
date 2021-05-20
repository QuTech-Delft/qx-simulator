#ifndef XPU_HASH_SET_H
#define XPU_HASH_SET_H


#include "qx/core/hash_table.h"

namespace xpu {
    namespace container
    {

        template <class __key, class __hasher = __hash_t<__key>, class __equalizer = equal_to<__key> >
                                                                                     class hash_set {

                                                                                     public:

            // Types
            typedef __hasher hasher;
            typedef __equalizer equal_key;
            typedef __key key_type;
            typedef __key value_type;
            typedef size_t size_type;

            // A key extraction object
            struct identity {
                const key_type& operator()(const value_type& value) const {
                    return value;
                }
            };

                                                                                     private:

            // Most implementation is in terms of a HashTable
            typedef hash_table<key_type, value_type, identity, hasher, equal_key> hash_table_t;

            // The HashTable
            hash_table_t ht;

                                                                                     public:

            // Iterator class is just an interator into the elements list
            typedef typename hash_table_t::iterator iterator;
            typedef typename hash_table_t::const_iterator const_iterator;

            // Constructors

            hash_set() { }

            hash_set(size_type n) : ht(n) { }

            hash_set(size_type n, const __hasher& hasher_) : ht(n, hasher_) { }

            hash_set(size_type n, const __hasher& hasher_, const __equalizer& equalizer_) : ht(n, hasher_, equalizer_) { }

            // Member functions

            iterator begin() {
                return ht.begin();
            }

            iterator end() {
                return ht.end();
            }

            const_iterator begin() const {
                return ht.begin();
            }

            const_iterator end() const {
                return ht.end();
            }

            size_type size() const {
                return ht.size();
            }

            size_type max_size() const {
                return ht.max_size();
            }

            size_type bucket_count() const {
                return ht.bucket_count();
            }

            bool empty() const {
                return ht.empty();
            }

            hasher hash_funct() const {
                return ht.hash_funct();
            }

            equal_key key_eq() const {
                return ht.key_eq();
            }

            void resize(size_type n) {
                ht.resize(n);
            }

            pair<iterator,bool> insert(const value_type& x) {
                return ht.insert(x);
            }

            void clear() {
                ht.clear();
            }

            iterator find(const key_type& k) {
                return ht.find(k);
            }

            const_iterator find(const key_type& k) const {
                return ht.find(k);
            }

            size_type count(const key_type& k) const {
                return ht.count(k);
            }

            void erase(iterator pos) {
                ht.erase(pos);
            }

            size_type erase(const key_type& k) {
                return ht.erase(k);
            }
        };


    } // namespace container
} // xpu

#endif




