// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2019, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2019, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <deque>
#include <string>

#include <sdsl/int_vector.hpp>

#include <seqan3/range/container/concatenated_sequences.hpp>
#include <seqan3/range/container/concept.hpp>

using namespace seqan3;

TEST(range_concept, ForwardRange)
{
    EXPECT_TRUE((std::ranges::ForwardRange<std::array<char, 2>>));
    EXPECT_TRUE((std::ranges::ForwardRange<std::list<char>>));
    EXPECT_TRUE((std::ranges::ForwardRange<std::forward_list<char>>)); // `.size()` missing
    EXPECT_TRUE((std::ranges::ForwardRange<std::vector<char>>));
    EXPECT_TRUE((std::ranges::ForwardRange<std::deque<char>>));
    EXPECT_TRUE((std::ranges::ForwardRange<std::string>));

    EXPECT_TRUE((std::ranges::ForwardRange<seqan3::concatenated_sequences<std::string>>));
    EXPECT_TRUE((std::ranges::ForwardRange<seqan3::concatenated_sequences<std::vector<char>>>));
}

TEST(Container, Container)
{
    EXPECT_TRUE((seqan3::Container<std::array<char, 2>>));
    EXPECT_TRUE((seqan3::Container<std::list<char>>));
    EXPECT_FALSE((seqan3::Container<std::forward_list<char>>)); // `.size()` missing
    EXPECT_TRUE((seqan3::Container<std::vector<char>>));
    EXPECT_TRUE((seqan3::Container<std::deque<char>>));
    EXPECT_TRUE((seqan3::Container<std::string>));

    EXPECT_TRUE((seqan3::Container<seqan3::concatenated_sequences<std::string>>));
    EXPECT_TRUE((seqan3::Container<seqan3::concatenated_sequences<std::vector<char>>>));
}

TEST(Container, sequence_container_former_travis_bug)
{
    // This tests a bug with const iterators on strings which was there in a
    // special gcc 7.2 build (ppa) for ubuntu 14.04 and 16.04. We leave it as
    // a regression test.
    // see https://github.com/seqan/seqan3/pull/113/
    using namespace std::string_literals;

    // example code from http://en.cppreference.com/w/cpp/string/basic_string/insert
    std::string s = "xmplr";

    // insert(size_type index, size_type count, char ch)
    s.insert(0, 1, 'E');
    EXPECT_EQ("Exmplr", s);

    // insert(size_type index, const char* s)
    s.insert(2, "e");
    EXPECT_EQ("Exemplr", s);

    // insert(size_type index, string const& str)
    s.insert(6, "a"s);
    EXPECT_EQ("Exemplar", s);

    // insert(size_type index, string const& str, size_type index_str, size_type count)
    s.insert(8, " is an example string."s, 0, 14);
    EXPECT_EQ("Exemplar is an example", s);

    // insert(const_iterator pos, char ch)
    s.insert(s.cbegin() + s.find_first_of('n') + 1, ':');
    EXPECT_EQ("Exemplar is an: example", s);

    // insert(const_iterator pos, size_type count, char ch)
    s.insert(s.cbegin() + s.find_first_of(':') + 1, 2, '=');
    EXPECT_EQ("Exemplar is an:== example", s);

    // insert(const_iterator pos, InputIt first, InputIt last)
    {
        std::string seq = " string";
        s.insert(s.cbegin() + s.find_last_of('e') + 1,
            std::begin(seq), std::end(seq));
        EXPECT_EQ("Exemplar is an:== example string", s);
    }

    // insert(const_iterator pos, std::initializer_list<char>)
    s.insert(s.cbegin() + s.find_first_of('g') + 1, { '.' });
    EXPECT_EQ("Exemplar is an:== example string.", s);
}

TEST(Container, SequenceContainer)
{
    EXPECT_FALSE((seqan3::SequenceContainer<std::array<char, 2>>));
    EXPECT_TRUE((seqan3::SequenceContainer<std::list<char>>));
    EXPECT_FALSE((seqan3::SequenceContainer<std::forward_list<char>>));
    EXPECT_TRUE((seqan3::SequenceContainer<std::vector<char>>));
    EXPECT_TRUE((seqan3::SequenceContainer<std::deque<char>>));
    EXPECT_TRUE((seqan3::SequenceContainer<std::string>));

    EXPECT_TRUE((seqan3::SequenceContainer<seqan3::concatenated_sequences<std::string>>));
    EXPECT_TRUE((seqan3::SequenceContainer<seqan3::concatenated_sequences<std::vector<char>>>));
}

TEST(Container, RandomAccessContainer)
{
    EXPECT_FALSE((seqan3::RandomAccessContainer<std::array<char, 2>>));
    EXPECT_FALSE((seqan3::RandomAccessContainer<std::list<char>>));
    EXPECT_FALSE((seqan3::RandomAccessContainer<std::forward_list<char>>));
    EXPECT_TRUE((seqan3::RandomAccessContainer<std::vector<char>>));
    EXPECT_TRUE((seqan3::RandomAccessContainer<std::deque<char>>));
    EXPECT_TRUE((seqan3::RandomAccessContainer<std::string>));

    EXPECT_TRUE((seqan3::RandomAccessContainer<seqan3::concatenated_sequences<std::string>>));
    EXPECT_TRUE((seqan3::RandomAccessContainer<seqan3::concatenated_sequences<std::vector<char>>>));
}

TEST(Container, ReservableContainer)
{
    EXPECT_FALSE((seqan3::ReservableContainer<std::array<char, 2>>));
    EXPECT_FALSE((seqan3::ReservableContainer<std::list<char>>));
    EXPECT_FALSE((seqan3::ReservableContainer<std::forward_list<char>>));
    EXPECT_TRUE((seqan3::ReservableContainer<std::vector<char>>));
    EXPECT_FALSE((seqan3::ReservableContainer<std::deque<char>>));
    EXPECT_TRUE((seqan3::ReservableContainer<std::string>));

    EXPECT_TRUE((seqan3::ReservableContainer<seqan3::concatenated_sequences<std::string>>));
    EXPECT_TRUE((seqan3::ReservableContainer<seqan3::concatenated_sequences<std::vector<char>>>));

    EXPECT_TRUE((seqan3::ReservableContainer<sdsl::bit_vector>));
    EXPECT_TRUE((seqan3::ReservableContainer<sdsl::int_vector<>>));
    EXPECT_TRUE((seqan3::ReservableContainer<sdsl::int_vector<13>>));
    EXPECT_TRUE((seqan3::ReservableContainer<sdsl::int_vector<64>>));
}

/* Check the SDSL containers */
//TODO

/* Check our containers */
//TODO
