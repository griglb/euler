// An electric circuit uses exclusively identical capacitors of the same value C.
// The capacitors can be connected in series or in parallel to form sub-units,
// which can then be connected in series or in parallel with other capacitors or
// other sub - units to form larger sub-units, and so on up to a final circuit.
//
// Using this simple procedureand up to n identical capacitors, we can make circuits
// having a range of different total capacitances.
// For example, using up to n=3 capacitors of 60 uF each, we can obtain the following
// 7 distinct total capacitance values:
//
//                          60uF
//                      +----||----+
//                      |   60uF   |
//              --------+----||----+--------    : 180 uF
//                      |   60uF   |
//                      +----||----+
//
//                          60uF
//                      +----||----+
//              --------+   60uF   +--------    : 120 uF
//                      +----||----+
//
//                      60uF     60uF
//                   +---||---.---||---+
//              -----+      60uF       +----    : 90 uF
//                   +-------||--------+
//
//                          60uF        
//              -------------||-------------    : 60 uF
//
//                              60uF
//                  60uF    +----||----+
//              -----||-----+   60uF   +----    : 40 uF
//                          +----||----+
//
//                    60uF       60uF
//              -------||----.----||--------    : 30 uF
//
//                   60uF   60uF   60uF
//              ------||--.--||--.--||------    : 20 uF
//
// If we denote by D(n) the number of distinct total capacitance values we can obtain
// when using up to n equal-valued capacitors and the simple procedure described above,
// we have:
//      D(1) = 1
//      D(2) = 3
//      D(3) = 7
// Find D(18).
//
// Reminder : When connecting capacitors C_1, C_2 etc in parallel, the total capacitance is
//      C_T = C_1 + C_2 + ...
// whereas when connecting them in series, the overall capacitance is given by:
//      1/C_T = 1/C_1 + 1/C_2 + ...


#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "fraction.h"


// One convenient feature of the capacitance relations is that they both
// support recursion.  Consider parallel circuits first.  If we have 4
// capacitors, they can be added in subsets and get the same result:
//      C_T = C_1 + C_2 + C_3 + C_4
//      C_T = ((C_1 + C_2) + C_3) + C_4
//      C_T = (C_1 + C_2) + (C_3 + C_4)
//      C_T = C_1 + (C_2 + (C_3 + C_4))
// The same is true for serial circuits:
//      1/C_T = 1/C_1 + 1/C_2 + 1/C_3 + 1/C_4
//  let 1/C_12 = 1/C_1 + 1/C_2
//      1/C_34 = 1/C_3 + 1/C_4
//      1/C_123 = 1/C_12 + 1/C_3 = 1/C_1 + 1/C_2 + 1/C_3
//      1/C_234 = 1/C_2 + 1/C_34 = 1/C_2 + 1/C_3 + 1/C_4
//  then
//      1/C_T = 1/C_123 + 1/C_4
//      1/C_T = 1/C_12 + 1/C_34
//      1/C_T = 1/C_1 + 1/C_234

// Let's rearrange the 7 solutions by the number of capacitors in them:
//      1 capacitor:
//
//                          60uF        
//              -------------||-------------    : 60 uF
//
//      2 capacitors:
//                          60uF
//                      +----||----+
//              --------+   60uF   +--------    : 120 uF
//                      +----||----+
//
//                    60uF       60uF
//              -------||----.----||--------    : 30 uF
//
//      3 capacitors:
//                          60uF
//                      +----||----+
//                      |   60uF   |
//              --------+----||----+--------    : 180 uF
//                      |   60uF   |
//                      +----||----+
//
//                      60uF     60uF
//                   +---||---.---||---+
//              -----+      60uF       +----    : 90 uF
//                   +-------||--------+
//
//                              60uF
//                  60uF    +----||----+
//              -----||-----+   60uF   +----    : 40 uF
//                          +----||----+
//
//                   60uF   60uF   60uF
//              ------||--.--||--.--||------    : 20 uF
//
// To go from 1 to 2, we take the single solution and add a new capacitor in
// both parallel and serial.
// To go from 2 to 3, we can take each of the 2 solutions and add a new
// capacitor in both parallel and serial.
// Another way to look at it is to take the sub-unit with 1 capacitor and the
// sub-units with 2 capacitors and put them in parallel and serial.

// Let's look at 4 capacitor solutions using the first approach, adding a
// single capacitor in parallel and serial to each of the 4 sub-units with 3:
// 
//                          60uF
//                      +----||----+
//                      |   60uF   |
//              --------+----||----+--------    : 240 uF
//                      |   60uF   |
//                      +----||----+
//                      |   60uF   |
//                      +----||----+
//
//                              60uF
//                          +----||----+
//                  60uF    |   60uF   |
//              -----||-----+----||----+----    : 45 uF
//                          |   60uF   |
//                          +----||----+
//
//                      60uF     60uF
//                   +---||---.---||---+
//              -----+      60uF       +----    : 150 uF
//                   +-------||--------+
//                   |      60uF       |
//                   +-------||--------+
//
//                             60uF     60uF
//                  60uF    +---||---.---||---+
//              -----||-----+      60uF       +----    : 36 uF
//                          +-------||--------+
//
//                                  60uF
//                       60uF   +----||----+
//              -----+----||----+   60uF   +----+----   : 100 uF
//                   |          +----||----+    |
//                   |           60uF           |
//                   +------------||------------+
//
//                                      60uF
//                  60uF    60uF    +----||----+
//              -----||------||-----+   60uF   +----    : 24 uF
//                                  +----||----+
//
//                       60uF   60uF   60uF
//              -----+----||--.--||--.--||----+-----    : 80 uF
//                   |          60uF          |
//                   +-----------||-----------+
//
//                   60uF   60uF   60uF   60uF
//              ------||--.--||--.--||--.--||------    : 15 uF
//
// This misses 2 configurations that can be formed by combining 2 of the 2 unit sub-units:
//                     60uF         60uF
//                  +---||---+   +---||---+
//              ----+  60uF  +---+  60uF  +----    : 60 uF
//                  +---||---+   +---||---+
//
//                      60uF       60uF
//              ----+----||----.----||----+----    : 60 uF
//                  |   60uF       60uF   |
//                  +----||----.----||----+
//
// In this case, both configurations have the same capacitance as a single unit, 60 uF,
// but that probably isn't always the case.


struct SubUnit {
    Fraction capacitance;
    std::vector<std::shared_ptr<SubUnit>> parallel_subs;
    std::vector<std::shared_ptr<SubUnit>> serial_subs;
};

using SubUnitPtr = std::shared_ptr<SubUnit>;

using Size = int16_t;
using UnitsOfSameSize = std::vector<SubUnitPtr>;
using UnitsBySize = std::map<Size, UnitsOfSameSize>;


std::ostream& operator<<(std::ostream& str, SubUnit unit) {
    str << "{" << unit.capacitance << ": ";
    if (!unit.parallel_subs.empty()) {
        str << "par(";
        for (const auto& sub : unit.parallel_subs)
            str << *sub;
        str << ")";
    }
    if (!unit.serial_subs.empty()) {
        str << "ser(";
        for (const auto& sub : unit.serial_subs)
            str << *sub;
        str << ")";
    }
    str << "}";

    return str;
}


UnitsBySize get_units(Size max_size) {
    UnitsBySize units;

    std::set<Fraction> all_caps;

    auto base_unit = std::make_shared<SubUnit>();
    base_unit->capacitance = Fraction{ 60, 1 };
    units[1] = { base_unit };
    all_caps.insert({ 60,1 });

    for (Size size = 2; size <= max_size; ++size) {
        std::cout << size << std::endl;
        units[size] = {};
        auto& new_units = units.at(size);

        for (Size sub_size = 1; sub_size <= size / 2; ++sub_size) {
            for (const auto& sub_unit_a : units.at(sub_size)) {
                for (const auto& sub_unit_b : units.at(size - sub_size)) {
                    SubUnit par_sub;
                    Fraction par_cap = sub_unit_a->capacitance + sub_unit_b->capacitance;
                    if (all_caps.find(par_cap) == all_caps.cend()) {
                        auto par_sub = std::make_shared<SubUnit>();
                        par_sub->capacitance = par_cap;
                        par_sub->parallel_subs.push_back(sub_unit_a);
                        par_sub->parallel_subs.push_back(sub_unit_b);
                        new_units.push_back(par_sub);
                        all_caps.insert(par_sub->capacitance);
                    }

                    SubUnit ser_sub;
                    Fraction ser_cap = (sub_unit_a->capacitance.inverse() + sub_unit_b->capacitance.inverse()).inverse();
                    if (all_caps.find(ser_cap) == all_caps.cend()) {
                        auto ser_sub = std::make_shared<SubUnit>();
                        ser_sub->capacitance = ser_cap;
                        ser_sub->serial_subs.push_back(sub_unit_a);
                        ser_sub->serial_subs.push_back(sub_unit_b);
                        new_units.push_back(ser_sub);
                        all_caps.insert(ser_sub->capacitance);
                    }
                }
            }
        }
    }

    return units;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto all_units = get_units(4);
    //    for (const auto& [size, units] : all_units) {
    //        std::cout << size << " : " << std::endl;
    //        for (const auto& un : units) {
    //            std::cout << "\t" << *un  << std::endl;
    //        }
    //    }
    //}

    {
        auto all_units = get_units(18);
        std::set<Fraction> all_caps;
        for (const auto& [size, units] : all_units) {
            for (const auto& un : units) {
                all_caps.insert(un->capacitance);
            }
        }

        //for (const auto& cap : all_caps) {
        //    std::cout << cap << std::endl;
        //}
        std::cout << "There are " << all_caps.size() << " unique capacitances" << std::endl;
    }
}
