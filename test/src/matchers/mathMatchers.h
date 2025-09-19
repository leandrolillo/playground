#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include "Math3d.h"


//TODO: Find a way to reuse this from math library.

template<typename Matrix>
struct EqualsMatrixMatcher: Catch::Matchers::MatcherGenericBase {
  EqualsMatrixMatcher(Matrix const &matrix) :
      matrix { matrix } {
  }

  template<typename OtherMatrix>
  bool match(OtherMatrix const &other) const {
    if (matrix.getNroFilas() != other.getNroFilas()
        || matrix.getNroColumnas() != other.getNroColumnas()) {
      return false;
    }
    for (unsigned int i = 0; i < matrix.getNroFilas(); i++) {
      for (unsigned int j = 0; i < matrix.getNroColumnas(); i++) {

        real absDifference = std::abs(matrix(i, j) - other(i, j));
        bool absoluteMargin=absDifference <= margin;
        bool relativeEpsilon=absDifference <= epsilon * std::max(matrix(i, j), other(i, j));

        if (!absoluteMargin && !relativeEpsilon) {
          if(!absoluteMargin) {
            printf("Abs(%.5f, %.5f) = %.5f  >   %.5f\n", matrix(i, j), other(i, j), absDifference, margin);
          }

          if(!relativeEpsilon) {
            printf("Abs(%.5f, %.5f) = %.5f  >   %.5f * %5f = %.5f\n", matrix(i, j), other(i, j), absDifference, epsilon, std::max(matrix(i, j), other(i, j)), epsilon * std::max(matrix(i, j), other(i, j)));
          }

          return false;
        }
      }
    }

    return true;
  }

  std::string describe() const override {
    return "EqualsMatrix: " + matrix.toString();
  }

private:
  Matrix const &matrix;
  real epsilon { 0.0000001 };
  real margin { MATH_EQUALS_MARGIN };
};

template<typename Matrix>
auto EqualsMatrix(const Matrix &range) -> EqualsMatrixMatcher<Matrix> {
  return EqualsMatrixMatcher<Matrix> { range };
}

template<typename Vector>
struct EqualsVectorMatcher: Catch::Matchers::MatcherGenericBase {
  EqualsVectorMatcher(Vector const &vector) :
      vector { vector } {
  }

  template<typename OtherVector>
  bool match(OtherVector const &other) const {
    if (vector.getLength() != other.getLength()) {
      return false;
    }
    for (unsigned int i = 0; i < vector.getLength(); i++) {
      real absDifference = std::abs(vector(i) - other(i));
      bool absoluteMargin=absDifference <= margin;
      bool relativeEpsilon=absDifference <= epsilon * std::max(vector(i), other(i));

      if (!absoluteMargin && !relativeEpsilon) {
        if(!absoluteMargin) {
          printf("Abs(%.5f, %.5f) = %.5f  >   %.5f\n", vector(i), other(i), absDifference, margin);
        }

        if(!relativeEpsilon) {
          printf("Abs(%.5f, %.5f) = %.5f  >   %.5f * %5f = %.5f\n", vector(i), other(i), absDifference, epsilon, std::max(vector(i), other(i)), epsilon * std::max(vector(i), other(i)));
        }
      }
    }

    return true;
  }

  std::string describe() const override {
    return "EqualsMatrix: " + vector.toString();
  }

private:
  Vector const &vector;
  real epsilon { 0.0001 };
  real margin { MATH_EQUALS_MARGIN };
};

template<typename Vector>
auto EqualsVector(const Vector &range) -> EqualsVectorMatcher<Vector> {
  return EqualsVectorMatcher<Vector> { range };
}
