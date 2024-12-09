#pragma once


template<typename C, typename E>
class Polynomial;

template<typename C, typename E>
Polynomial<C, E> operator+(const Polynomial<C, E>& p1, const Polynomial<C, E>& p2);

template<typename C, typename E>
Polynomial<C, E> operator-(const Polynomial<C, E>& p1, const Polynomial<C, E>& p2);

template<typename C, typename E>
Polynomial<C, E> operator*(const Polynomial<C, E>& p1, const Polynomial<C, E>& p2);

template<typename C, typename E, typename S>
Polynomial<C, E> operator*(const Polynomial<C, E>& p1, const S scale);

template<typename C, typename E>
Polynomial<C, E> operator^(const Polynomial<C, E>& p1, const E exp);


template <typename CoefficientT, typename ExponentT>
class Polynomial {
public:
    // Use signed values for exponent, to allow for 1/x^n.
    Polynomial(const std::string& variable = { "x" }) : variable_(variable) { }
    ~Polynomial() = default;

    Polynomial(const Polynomial& other) = default;// : terms_(other.terms_.cbegin(), other.terms_.cend()) { }
    Polynomial& operator=(const Polynomial& rhs) = delete;

    Polynomial(Polynomial&& other) = default;
    Polynomial& operator=(Polynomial&& rhs) = default;

    void AddTerm(CoefficientT coeff, ExponentT exp) {
        terms_[exp] = coeff;
    }

    CoefficientT SumOfCoefficients() const {
        CoefficientT ret;

        for (const auto& [exp, coeff] : terms_) {
            ret += coeff;
        }

        return ret;
    }

    double Evaluate(double var) const {
        double ret{ 0 };

        for (const auto& [exp, coeff] : terms_) {
            if (0 == exp) {
                ret += coeff.to_double();
            }
            else {
                double term = coeff.to_double() * 1.0;
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
                    ExponentT prod_exp = lhs_exp + rhs_exp;
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
                    ExponentT prod_exp = lhs_exp + rhs_exp;
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

    template <typename S>
    Polynomial& operator*=(const S scale) {
        for (auto& [exp, coeff] : terms_) {
            coeff *= scale;
        }

        return *this;
    }

    Polynomial GetNormalizer() const {
        Polynomial ret;

        const ExponentT smallest = terms_.rbegin()->first;
        if (smallest < 0)
            ret.AddTerm(CoefficientT{ 1 }, -smallest);
        else
            ret.AddTerm(CoefficientT{ 1 }, 0);

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
        subst.AddTerm(CoefficientT{ 1 }, 1);
        subst.AddTerm(CoefficientT{ -1 }, -1);

        for (const auto& [exp, coeff] : terms_) {
            if (0 == exp) {
                Polynomial term;
                term.AddTerm(CoefficientT{ coeff }, 0);
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
        Polynomial ret{ "x" };

        for (const auto& [exp, coeff] : terms_) {
            ret.AddTerm(CoefficientT{ coeff }, exp / 2);
        }

        return ret;
    }

    Polynomial Derivative() const {
        Polynomial ret{ variable_ };

        for (const auto& [exp, coeff] : terms_) {
            if (exp > 0) {
                ret.AddTerm(CoefficientT{ coeff * exp }, exp - 1);
            }
        }

        return ret;
    }

    void Rescale() {
        // If any terms have a negative exponent, multiply through my its inverse to make all non-negative.
        if (terms_.rbegin()->first < 0) {
            const ExponentT shift = -terms_.rbegin()->first;
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
            str << ((coeff > CoefficientT{ 0 }) ? "+ " : "- ");
            str << abs(coeff) << " " << variable_ << "^" << exp << " ";
            //str << std::abs(coeff) << " " << variable_ << "^" << exp << " ";
        }
        return str.str();
    }

private:
    void CleanupTerms() {
        std::vector<ExponentT> zeroes;
        for (const auto& [exp, coeff] : terms_) {
            if (CoefficientT{ 0 } == coeff)
                zeroes.push_back(exp);
        }
        for (const auto& exp : zeroes)
            terms_.erase(exp);
    }

    // Use a map to store the coefficients by power of x.
    // If the polynomial is a_4 x^4 + a_3 x^3 + a_2 x^2 + a_1 x + a_0,
    // then the map is { {4, a_4}, {3, a_3}, {2, a_2}, {1, a_1}, {0, a_0} }.
    using Terms = std::map<ExponentT, CoefficientT, std::greater<ExponentT>>;

    std::string variable_;
    Terms terms_;
};


template<typename C, typename E>
Polynomial<C, E> operator+(const Polynomial<C, E>& p1, const Polynomial<C, E>& p2) {
    Polynomial<C, E> ret = p1;
    ret += p2;
    return ret;
}


template<typename C, typename E>
Polynomial<C, E> operator-(const Polynomial<C, E>& p1, const Polynomial<C, E>& p2) {
    Polynomial<C, E> ret = p1;
    ret -= p2;
    return ret;
}


template<typename C, typename E>
Polynomial<C, E> operator*(const Polynomial<C, E>& p1, const Polynomial<C, E>& p2) {
    Polynomial<C, E> ret = p1;
    ret *= p2;
    return ret;
}


template<typename C, typename E, typename S>
Polynomial<C, E> operator*(const Polynomial<C, E>& p1, const S scale) {
    Polynomial<C, E> ret = p1;
    ret *= scale;
    return ret;
}


template<typename C, typename E>
Polynomial<C, E> operator^(const Polynomial<C, E>& p1, const E exp) {
    Polynomial<C, E> ret = p1;
    for (E i = 1; i < exp; ++i) {
        ret *= p1;
    }
    return ret;
}
