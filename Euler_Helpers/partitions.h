#pragma once


#include <cstddef>
#include <map>
#include <vector>


using Group = std::vector<size_t>;
using Partition = std::vector<Group>;
using Partitions = std::vector<Partition>;


class Partitioner {
public :
    Partitioner();
    ~Partitioner() = default;

    const Partitions &get_partitions(size_t num_elem);

    size_t get_max_partition_size() const { return partition_memo_.size(); }

private :
	using PartitionMemo = std::map<size_t, Partitions>;
	PartitionMemo partition_memo_;

    void calc_partitions(size_t num_elem);

};