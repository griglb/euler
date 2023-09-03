#include "partitions.h"

// Generating partitions is more easily done recursively.
// Let's start with a single element 0, there is only 1 partition:
//      { 0 }
// Now move to 2 elements 0, 1.  We can take the list of partitions for 1 element
// and add the new element to each partition as part of any subset or a new subset:
//      { 0 }  =>  { 0, 1 }  or  { 0 }, { 1 }
// The pattern becomes a little more obvious with 3 elements 0, 1, 2:
//      { 0, 1 }        =>  { 0, 1, 2 }  or  { 0, 1 }, { 2 }
//      { 0 }, { 1 }    =>  { 0, 2 }, { 1 }  or  { 0 }, { 1, 2 }  or  { 0 }, { 1 }, { 2 }
// To continue with 4 elements:
//      { 0, 1, 2 }             => { 0, 1, 2, 3}  or  { 0, 1, 2 }, { 3 }
//      { 0, 1 }, { 2 }         => { 0, 1, 3 }, { 2 }  or  { 0, 1 }, { 2, 3 }  or  { 0, 1 }, { 2 }, { 3 }
//      { 0, 2 }, { 1 }         => { 0, 2, 3 }, { 1 }  or  { 0, 2 }, { 1, 3 }  or  { 0, 2 }, { 1 }, { 3 }
//      { 0 }, { 1, 2 }         => { 0, 3 }, { 1, 2 }  or  { 0 }, { 1, 2, 3 }  or  { 0 }, { 1, 2 }, { 3 }
//      { 0 }, { 1 }, { 2 }     => { 0, 3 }, { 1 }, { 2 }  or  { 0 }, { 1, 3 }, { 2 }  or  { 0 }, { 1 }, { 2, 3 }  or  { 0 }, { 1 }, { 2 }, { 3 }


Partitioner::Partitioner() {
    // Seed the partition map with the single partition for a single element set.
    partition_memo_[1] = { { { 0 } } };
}


const Partitions &Partitioner::get_partitions(size_t num_elem) {
    if (partition_memo_.find(num_elem) == partition_memo_.cend())
        calc_partitions(num_elem);

    return partition_memo_.at(num_elem);
}


void Partitioner::calc_partitions(size_t num_elem) {
    if (partition_memo_.find(num_elem) != partition_memo_.cend())
        return;

    if (partition_memo_.find(num_elem - 1) == partition_memo_.cend())
        calc_partitions(num_elem - 1);

    const auto& sub_parts = partition_memo_.at(num_elem - 1);
    const size_t new_elem{ num_elem - 1 };

    partition_memo_[num_elem] = { };
    auto& new_parts = partition_memo_.at(num_elem);

    for (const auto& sub_part : sub_parts) {
        for (size_t i = 0; i < sub_part.size(); ++i) {
            auto new_part = sub_part;
            new_part[i].push_back(new_elem);
            new_parts.emplace_back(new_part);
        }

        auto new_part = sub_part;
        new_part.push_back({ new_elem });
        new_parts.emplace_back(new_part);
    }
}
