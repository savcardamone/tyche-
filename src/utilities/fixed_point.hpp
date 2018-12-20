/**
 * @file fixed_point.hpp
 * @author Salvatore Cardamone
 * @brief Largely a replica of the Fixed Point Math Library developed by
 *        Peter Schregle. Pared down superfluous functionality and added some
 *        bits and pieces to make the code a little more useful for the types
 *        of calculation we do in tyche++.
 */
#ifndef __TYCHEPLUSPLUS_FIXED_POINT_HPP
#define __TYCHEPLUSPLUS_FIXED_POINT_HPP

#include <boost/operators.hpp>

namespace tycheplusplus {

/**
 * @class FixedPoint
 * @brief Generic fixed point functionality, allowing us to use this class as
 *        we would any other data type, like float or double. All integer
 *        arithmetic, so if there's no dedicated FPU, using FixedPoint as the
 *        real numerical type is probably advantageous. Furthermore, any
 *        high-level synthesis tools will hopefully pick up on the use of
 *        integer arithmetic and generate efficient implementations.
 *
 *        boost/operators.hpp provides a fairly remarkable set of
 *        functionalities, whereby operators can be automatically generated from
 *        a smaller set of operators. Deriving from the appropriate boost
 *        classes within boost/operators.hpp then gives us access to the full
 *        complement of operators.
 *
 *        For instance, deriving from boost::ordered_field_operators allows us
 *        to explicitly define the += operator, and obtain the + operator for
 *        free, without the need for additional boilerplate.
 *
 * @tparam S Data type used to store the fixed point number. It the type is
 *           signed, then the fixed point representation will be signed too.
 * @tparam I Number of integer bits.
 * @tparam F Number of fractional bits. Automatically determined from number of
 *           integer bits and number of available storage bits.
 */
template<typename B,
         unsigned char I,
         unsigned char F = std::numeric_limits<B>::digits - I>
class FixedPoint
    : boost::ordered_field_operators<FixedPoint<B,I,F>,
      boost::unit_steppable<FixedPoint<B,I,F>,
      boost::shiftable<FixedPoint<B,I,F> > > >
    {

public:
  typedef B base_type_;
  static constexpr unsigned char number_integer_bits_ = I;
  static constexpr unsigned char number_fractional_bits_ = F;
  static constexpr unsigned long long two_power_f = (1ULL << F);

  // Alias for the sake of simplifying functions
  using fixed = FixedPoint<B,I,F>;

  FixedPoint(float value)
      : value_(value * two_power_f + value >= 0 ? 0.5 : -0.5) {}

  /**
   * @brief Class constructor.
   * @param value Double precision value to initialise with.
   */
  FixedPoint(double value)
      : value_(value * two_power_f + value >= 0 ? 0.5 : -0.5) {}

  /**
   * @brief Class constructor.
   * @param value Single precision value to initialise with.
   */
  fixed& operator +=(fixed const& rhs) {
    value_ += rhs.value_;
    return *this;
  }

  /**
   * @brief Subtraction assignment operator.
   * @param rhs Value to subtract from lhs.
   * @retval lhs - rhs.
   */
  fixed& operator -=(fixed const& rhs) {
    value_ -= rhs.value_;
    return *this;
  }


private:
  B value_;

} ;

}

#endif /* #ifndef __TYCHEPLUSPLUS_FIXED_POINT_HPP */
