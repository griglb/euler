// Consider the sequence of real numbers a_n defined by the starting value a_0 and the recurrence
//      a_n+1 = a_n - 1 / a_n for any n >= 0.
//
// For some starting values a_0 the sequence will be periodic.
// For example, a_0 = sqrt(1/2) yields the sequence :
//      sqrt(1/2), -sqrt(1/2), sqrt(1/2), ...
// 
// We are interested in the range of such a periodic sequence which is the difference between
// the maximum and minimum of the sequence.
// For example, the range of the sequence above would be
//      sqrt(1/2) - (-sqrt(1/2)) = sqrt(2).
//
// Let S(P) be the sum of the ranges of all such periodic sequences with a period not exceeding P.
// For example, S(2) = 2 sqrt(2) ~= 2.8284, being the sum of the ranges of the two sequences
// starting with a_0 = sqrt(1/2) and a_0 = -sqrt(1/2).
//
// You are given 
//      S(3) ~= 14.6461
//      S(5) ~= 124.1056
//
// Find S(25), rounded to 4 decimal places.


#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using Exponent = int16_t;
using Coefficient = double;

class Polynomial;

Polynomial operator+(const Polynomial& p1, const Polynomial& p2);
Polynomial operator-(const Polynomial& p1, const Polynomial& p2);
Polynomial operator*(const Polynomial& p1, const Polynomial& p2);
Polynomial operator*(const Polynomial& p1, const double scale);
Polynomial operator^(const Polynomial& p1, const Exponent exp);


class Polynomial {
public:
    // Use signed values for exponent, to allow for 1/x^n.
    Polynomial(const std::string& variable = {"x"}) : variable_(variable) { }
    ~Polynomial() = default;

    Polynomial(const Polynomial& other) = default;// : terms_(other.terms_.cbegin(), other.terms_.cend()) { }
    Polynomial& operator=(const Polynomial& rhs) = delete;

    Polynomial(Polynomial&& other) = default;
    Polynomial& operator=(Polynomial&& rhs) = default;

    void AddTerm(Coefficient coeff, Exponent exp) {
        terms_[exp] = coeff;
    }

    double Evaluate(double var) const {
        double ret{ 0 };

        for (const auto& [exp, coeff] : terms_) {
            if (0 == exp) {
                ret += coeff;
            }
            else {
                double term = coeff * 1.0;
                for (int i = 0; i < exp; ++i)
                    term *= var;
                ret += term;
            }
        }

        return ret;
    }

    Polynomial& operator+=(const Polynomial& rhs) {
        for (const auto& [exp, coeff] : rhs.terms_) {
            if (terms_.find(exp) == terms_.cend())
                terms_[exp] = coeff;
            else
                terms_[exp] += coeff;
        }
        CleanupTerms();
        return *this;
    }

    Polynomial& operator-=(const Polynomial& rhs) {
        for (const auto& [exp, coeff] : rhs.terms_) {
            if (terms_.find(exp) == terms_.cend())
                terms_[exp] = -coeff;
            else
                terms_[exp] -= coeff;
        }
        CleanupTerms();
        return *this;
    }

    Polynomial& operator*=(const Polynomial& rhs) {
        // First move my terms into a temp map, so my terms can be replaced.
        Terms orig_terms;
        terms_.swap(orig_terms);

        if (&rhs == this) {
            // If multiplied by self, use copy of terms for both loops.
            for (const auto& [lhs_exp, lhs_coeff] : orig_terms) {
                for (const auto& [rhs_exp, rhs_coeff] : orig_terms) {
                    Exponent prod_exp = lhs_exp + rhs_exp;
                    if (terms_.find(prod_exp) == terms_.cend())
                        terms_[prod_exp] = lhs_coeff * rhs_coeff;
                    else
                        terms_[prod_exp] += lhs_coeff * rhs_coeff;
                }
            }
        }
        else {
            for (const auto& [lhs_exp, lhs_coeff] : orig_terms) {
                for (const auto& [rhs_exp, rhs_coeff] : rhs.terms_) {
                    Exponent prod_exp = lhs_exp + rhs_exp;
                    if (terms_.find(prod_exp) == terms_.cend())
                        terms_[prod_exp] = lhs_coeff * rhs_coeff;
                    else
                        terms_[prod_exp] += lhs_coeff * rhs_coeff;
                }
            }
        }
        CleanupTerms();
        return *this;
    }

    Polynomial& operator*=(const double scale) {
        for (auto& [exp, coeff] : terms_) {
            coeff *= scale;
        }

        return *this;
    }

    Polynomial GetNormalizer() const {
        Polynomial ret;

        const Exponent smallest = terms_.rbegin()->first;
        if (smallest < 0)
            ret.AddTerm(1, -smallest);
        else
            ret.AddTerm(1, 0);

        return ret;
    }

    Polynomial SubstituteA_n() const {
        int index = 0;
        {
            std::stringstream str{ variable_ };
            char c0, c1;
            str >> c0 >> c1 >> index;
            if (('a' != c0) || ('_' != c1))
                throw "what?";
        }
        std::string new_var;
        {
            std::stringstream str;
            str << "a_" << index - 1;
            new_var = str.str();
        }
        Polynomial ret{ new_var };

        Polynomial subst;
        subst.AddTerm(1, 1);
        subst.AddTerm(-1, -1);

        for (const auto& [exp, coeff] : terms_) {
            if (0 == exp) {
                Polynomial term;
                term.AddTerm(coeff, 0);
                ret += term;
            }
            else if (1 == exp) {
                ret += subst * coeff;
            }
            else {
                Polynomial term = subst ^ exp;
                ret += term * coeff;
            }
        }

        return ret;
    }

    Polynomial SubstituteXForA2() const {
        Polynomial ret{ "x"};

        for (const auto& [exp, coeff] : terms_) {
            ret.AddTerm(coeff, exp / 2);
        }

        return ret;
    }

    Polynomial Derivative() const {
        Polynomial ret{ variable_ };

        for (const auto& [exp, coeff] : terms_) {
            if (exp > 0) {
                ret.AddTerm(coeff * exp, exp - 1);
            }
        }

        return ret;
    }

    void Rescale() {
        // If any terms have a negative exponent, multiply through my its inverse to make all non-negative.
        if (terms_.rbegin()->first < 0) {
            const Exponent shift = -terms_.rbegin()->first;
            Terms orig_terms;
            terms_.swap(orig_terms);
            for (const auto& [exp, coeff] : orig_terms) {
                terms_[exp + shift] = coeff;
            }
        }
    }

    std::string DumpEquation() const {
        if (terms_.empty())
            return "0";

        std::stringstream str;
        for (const auto& [exp, coeff] : terms_) {
            str << ((coeff > 0) ? "+ " : "- ");
            str << std::abs(coeff) << " " << variable_ << "^" << exp << " ";
        }
        return str.str();
    }

private:
    void CleanupTerms() {
        std::vector<Exponent> zeroes;
        for (const auto& [exp, coeff] : terms_) {
            if (0 == coeff)
                zeroes.push_back(exp);
        }
        for (const auto& exp : zeroes)
            terms_.erase(exp);
    }

    // Use a map to store the coefficients by power of x.
    // If the polynomial is a_4 x^4 + a_3 x^3 + a_2 x^2 + a_1 x + a_0,
    // then the map is { {4, a_4}, {3, a_3}, {2, a_2}, {1, a_1}, {0, a_0} }.
    using Terms = std::map<Exponent, Coefficient, std::greater<Exponent>>;

    std::string variable_;
    Terms terms_;
};


Polynomial operator+(const Polynomial& p1, const Polynomial& p2) {
    Polynomial ret = p1;
    ret += p2;
    return ret;
}


Polynomial operator-(const Polynomial& p1, const Polynomial& p2) {
    Polynomial ret = p1;
    ret -= p2;
    return ret;
}


Polynomial operator*(const Polynomial& p1, const Polynomial& p2) {
    Polynomial ret = p1;
    ret *= p2;
    return ret;
}


Polynomial operator*(const Polynomial& p1, const double scale) {
    Polynomial ret = p1;
    ret *= scale;
    return ret;
}


Polynomial operator^(const Polynomial& p1, const Exponent exp) {
    Polynomial ret = p1;
    for (Exponent i = 1; i < exp; ++i) {
        ret *= p1;
    }
    return ret;
}


Polynomial EquationForS_n(int16_t n) {
    std::string var_name;
    {
        std::stringstream str;
        str << "a_" << n - 1;
        var_name = str.str();
    }

    Polynomial lhs{ var_name }, rhs{ var_name };
    // To start, lhs = 1, rhs = x - 1/x
    lhs.AddTerm(1, 0);
    rhs.AddTerm(1, 1);
    rhs.AddTerm(-1, -1);

    for (int16_t i = 1; i < n; ++i) {
        std::cout << "LHS = " << lhs.DumpEquation() << ", RHS = " << rhs.DumpEquation() << std::endl;

        const Polynomial norm = rhs.GetNormalizer();
        lhs *= norm;
        rhs *= norm;

        std::cout << "LHS = " << lhs.DumpEquation() << ", RHS = " << rhs.DumpEquation() << std::endl;

        lhs = lhs.SubstituteA_n();
        rhs = rhs.SubstituteA_n();
    }

    Polynomial a0;
    a0.AddTerm(1, 1);
    lhs *= a0;

    lhs -= rhs;

    lhs.Rescale();

    return lhs.SubstituteXForA2();
}


std::vector<double> SolveEquation(const Polynomial& poly) {
    // Use Netwon-Raphson method
    //      x_n+1 = x_n - f(x_n) / f'(x_n)

    const Polynomial deriv = poly.Derivative();

    double x_n = 0.0;
    double x_np1 = x_n - poly.Evaluate(x_n) / deriv.Evaluate(x_n);

    while (std::abs(x_np1 - x_n) > 1.0E-8) {
        x_n = x_np1;
        x_np1 = x_n - poly.Evaluate(x_n) / deriv.Evaluate(x_n);
    }

    std::vector<double> roots;

    double a_0 = std::sqrt(x_np1);
    roots.push_back(a_0);

    double a_n = a_0;
    for (double a_np1 = a_n - 1.0 / a_n; std::abs(a_np1 - a_0) > 1.0E-5; a_n = a_np1, a_np1 = a_n - 1.0 / a_n)
        roots.push_back(a_np1);

    return roots;
}

int main()
{
    std::cout << "Hello World!\n";

/* {
        {
            Polynomial p;
            p.AddTerm(4, 2);
            p.AddTerm(-3, 1);
            std::cout << p.DumpEquation() << std::endl;
        }

        {
            Polynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-3, 2);
            p2.AddTerm(5, 0);
            p1 += p2;
            std::cout << "p1 + p2 = " << p1.DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            Polynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-3, 2);
            p2.AddTerm(5, 0);
            p1 -= p2;
            std::cout << "p1 - p2 = " << p1.DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            Polynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-3, 2);
            p2.AddTerm(5, 0);
            p1 *= p2;
            std::cout << "p1 * p2 = " << p1.DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            Polynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-4, 2);
            p2.AddTerm(5, 0);
            p1 += p2;
            std::cout << "p1 + p2 = " << p1.DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            Polynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p1 += p1;
            std::cout << "p1 + p1 = " << p1.DumpEquation() << std::endl;
        }

        {
            Polynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p1 -= p1;
            std::cout << "p1 - p1 = " << p1.DumpEquation() << std::endl;
        }

        {
            Polynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p1 *= p1;
            std::cout << "p1 * p1 = " << p1.DumpEquation() << std::endl;
        }

        {
            Polynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-3, 2);
            p2.AddTerm(5, 0);
            std::cout << "p1 + p2 = " << (p1 + p2).DumpEquation() << std::endl;
            std::cout << "p1 - p2 = " << (p1 - p2).DumpEquation() << std::endl;
            std::cout << "p1 * p2 = " << (p1 * p2).DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            Polynomial p;
            p.AddTerm(1, 1);
            p.AddTerm(-1, -1);
            std::cout << "p^2 = " << (p ^ 2).DumpEquation() << std::endl;
            std::cout << "p^3 = " << (p ^ 3).DumpEquation() << std::endl;
            std::cout << "p^4 = " << (p ^ 4).DumpEquation() << std::endl;
            Polynomial p2 = p ^ 5;
            p2.Rescale();
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }
    }*/

/*    {
        Polynomial lhs2("a_2"), rhs2("a_2");
        // To start, lhs = 1, rhs = x - 1/x
        lhs2.AddTerm(1, 0);
        rhs2.AddTerm(1, 1);
        rhs2.AddTerm(-1, -1);

        std::cout << "LHS2 = " << lhs2.DumpEquation() << ", RHS2 = " << rhs2.DumpEquation() << std::endl;

        const Polynomial norm2 = rhs2.GetNormalizer();
        lhs2 *= norm2;
        rhs2 *= norm2;

        std::cout << "LHS2 = " << lhs2.DumpEquation() << ", RHS2 = " << rhs2.DumpEquation() << std::endl;

        Polynomial lhs1 = lhs2.SubstituteA_n();
        Polynomial rhs1 = rhs2.SubstituteA_n();

        std::cout << "LHS1 = " << lhs1.DumpEquation() << ", RHS1 = " << rhs1.DumpEquation() << std::endl;

        const Polynomial norm1 = rhs1.GetNormalizer();
        lhs1 *= norm1;
        rhs1 *= norm1;

        std::cout << "LHS1 = " << lhs1.DumpEquation() << ", RHS1 = " << rhs1.DumpEquation() << std::endl;

        Polynomial lhs0 = lhs1.SubstituteA_n();
        Polynomial rhs0 = rhs1.SubstituteA_n();

        std::cout << "LHS0 = " << lhs0.DumpEquation() << ", RHS0 = " << rhs0.DumpEquation() << std::endl;

        const Polynomial norm0 = rhs0.GetNormalizer();
        lhs0 *= norm0;
        rhs0 *= norm0;

        std::cout << "LHS0 = " << lhs0.DumpEquation() << ", RHS0 = " << rhs0.DumpEquation() << std::endl;

        Polynomial a0;
        a0.AddTerm(1, 1);
        lhs0 *= a0;

        lhs0 -= rhs0;
        std::cout << "LHS0 = " << lhs0.DumpEquation() << std::endl;
    }*/

    {
        Polynomial poly = EquationForS_n(7);
        std::cout << poly.DumpEquation() << std::endl;
        std::cout << poly.Derivative().DumpEquation() << std::endl;
        std::cout << "f(1) = " << poly.Evaluate(1.0) << std::endl;
        std::cout << "f(2) = " << poly.Evaluate(2.0) << std::endl;
        std::cout << "f(0.5) = " << poly.Evaluate(0.5) << std::endl;
        std::cout << "f(-1) = " << poly.Evaluate(-1.0) << std::endl;
        auto roots = SolveEquation(poly);
        for (const auto& r : roots)
            std::cout << r << " = " << poly.Evaluate(r*r) << std::endl;
        Polynomial subPoly;
        subPoly.AddTerm(1, 0);
        for (const auto& r : roots) {
            Polynomial mono;
            mono.AddTerm(1, 1);
            mono.AddTerm(-r * r, 0);
            subPoly *= mono;
        }
        std::cout << subPoly.DumpEquation() << std::endl;

    }

}
