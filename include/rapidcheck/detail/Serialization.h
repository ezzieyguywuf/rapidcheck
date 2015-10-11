#pragma once

#include <stdexcept>
#include <string>

namespace rc {
namespace detail {

class SerializationException : public std::exception {
public:
  /// C-tor.
  ///
  /// @param msg  A message describing the serialization error.
  SerializationException(const std::string &msg);

  /// Returns the message.
  std::string message() const;

  const char *what() const noexcept override;

private:
  std::string m_msg;
};

/// Serializes the given integer value in little-endian format.
///
/// @param value   The value to serialize.
/// @param output  An output iterator to write bytes to.
///
/// @return An iterator pointing to after the written data.
template <typename T,
          typename Iterator,
          typename = typename std::enable_if<std::is_integral<T>::value>::type>
Iterator serialize(T value, Iterator output);

/// Deserializes integers as serialized by `serialize`.
///
/// @param begin  An iterator pointing to the start of the data range.
/// @param end    An iterator pointing to the end of the data range.
/// @param out    A reference to store the output in.
///
/// @return An iterator pointing to after the consumed data.
/// @throws SerializationException  On deserialization failure.
template <typename T,
          typename Iterator,
          typename = typename std::enable_if<std::is_integral<T>::value>::type>
Iterator deserialize(Iterator begin, Iterator end, T &out);

/// Serializes `n` number of elements from the `in` without storing the length.
/// Thus to deserialize, the exact number of elements must be known beforehand.
///
/// @param in   The input iterator.
/// @param n    The number of elements to serialize.
/// @param out  The output iterator.
template <typename InputIterator, typename OutputIterator>
OutputIterator serializeN(InputIterator in, std::size_t n, OutputIterator out);

/// Deserializes `n` number of elements of type `T`.
///
/// @param begin  The start of the input range.
/// @param end    The end of the input range.
/// @param n      The number of elements to deserialize.
/// @param out    The output iterator.
///
/// @return Iterator past the consumed data.
/// @throws SerializationException  On deserialization failure.
template <typename T, typename InputIterator, typename OutputIterator>
InputIterator deserializeN(InputIterator begin,
                           InputIterator end,
                           std::size_t n,
                           OutputIterator out);

/// Serializes the given integer value in a compact form where only as many
/// bits
/// as needed are used.
///
/// This is done by using only seven bits in each byte. When high bit is
/// set, it
/// indicates that there are more bytes to read. This format is only useful
/// if
/// the number of bits required is usually low. If the higher bits are
/// usually
/// set, it will lead to inefficient storage.
///
/// @param value   The value to serialize.
/// @param output  An output iterator to write bytes to.
///
/// @return An iterator pointing to after the written data.
template <typename T, typename Iterator>
Iterator serializeCompact(T value, Iterator output);

/// Deserializes integers as serialized by `serializeCompact`.
///
/// @param begin   The start iterator of the data to deserialize.
/// @param end     The end iterator of the data to deserialize.
/// @param output  Reference to store the output into.
///
/// @return Iterator pointing to the rest of the data that was not consumed.
/// @throws SerializationException  On deserialization failure.
template <typename T, typename Iterator>
Iterator deserializeCompact(Iterator begin, Iterator end, T &output);

/// Serializes the given range of integers to the given output iterator. The
/// output is serialized using `serializeCompact` with a prefix length also
/// serialized the same way.
///
/// @param begin   The start of the range to serialize.
/// @param end     The end of the range to serialize.
/// @param output  The output iterator to serialize to.
///
/// @return An iterator past the end of the output.
template <typename InputIterator, typename OutputIterator>
OutputIterator
serializeCompact(InputIterator begin, InputIterator end, OutputIterator output);

/// Deserializes a range of integers as serialized by the range version of
/// `serializeCompact`.
///
/// @param begin  The start of the data to deserialize.
/// @param begin  The end of the data to deserialize.
///
/// @return A pair of iterators pointing past the end of the consumed data and
///         past the end of the output data, respectively.
/// @throws SerializationException  On deserialization failure.
template <typename T, typename InputIterator, typename OutputIterator>
std::pair<InputIterator, OutputIterator> deserializeCompact(
    InputIterator begin, InputIterator end, OutputIterator output);

} // namespace detail
} // namespace rc

#include "Serialization.hpp"