
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_ALGORITHM_CLASSES
#include "algorithm_classes.h"
#undef COMPILE_TEMPLATE_ALGORITHM_CLASSES

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <set>
#include <tuple>
#include <vector>
#include <iostream>

#include "algorithm_map_operations.h"
#include "algorithm_row_operations.h"

using namespace panda;

template <typename Integer, typename TagType>
Row<Integer> panda::algorithm::classRepresentative(const Row<Integer>& row, const Maps& maps, TagType tag)
{
   assert( !row.empty() );
   const auto matrix = getClass(row, maps, tag);
   assert( !matrix.empty() );
   return *matrix.crbegin(); // Important detail: last element is chosen as the representative
}

template <typename Integer, typename TagType>
std::set<Row<Integer>> panda::algorithm::getClass(const Row<Integer>& row, const Maps& maps, TagType tag)
{
   assert( !row.empty() );
   std::set<Row<Integer>> rows;
   rows.insert(row);
   using Iterator = typename std::set<Row<Integer>>::iterator;
   std::vector<Iterator> iterators;
   iterators.push_back(rows.begin());
   while ( !iterators.empty() )
   {
      const auto& current_row = *iterators.back();
      iterators.pop_back();
      for ( const auto& map : maps )
      {
         const auto new_row = apply(map, current_row, tag);
         Iterator iterator;
         bool inserted;
         std::tie(iterator, inserted) = rows.insert(new_row);
         if ( inserted )
         {
            iterators.push_back(iterator);
         }
      }
   }
   return rows;
}

template <typename Integer, typename TagType>
Matrix<Integer> panda::algorithm::classes(Matrix<Integer> input, const Maps& maps, TagType tag)
{
   std::set<Row<Integer>> rows(input.cbegin(), input.cend());
   return classes(rows, maps, tag);
}

template <typename Integer, typename TagType>
Matrix<Integer> panda::algorithm::classes(std::set<Row<Integer>> rows, const Maps& maps, TagType tag)
{
   Matrix<Integer> classes;
   // shift the rows
   auto rows_shifted = affineTransformation(rows);
   std::cout << "Shifted all rows";
   // while the rows are not empty
   while ( !rows.empty() )
   {
      const auto row_class = getClass(*rows.begin(), maps, tag);
      // get the shifted version of the row classes
      const auto row_shifted_class = affineTransformation(row_class);
      assert( !row_class.empty() );
      assert( !row_shifted_class.empty() );
      classes.push_back(*row_class.crbegin()); // Important detail: last element is chosen as the representative
      // Iterate over the class rows (and shifted ones just by index)
      for ( auto& row_shifted : row_shifted_class )
      {
         // set idx -> just lazy could also change the for loop
         const auto iterator = row_shifted_class.find(row_shifted);
         // TODO: The above statement needs to be replaced by a function that checks equivalence by our definition
         // auto position = checkEquivalence(rows_shifted, row_shifted);
         if ( iterator != row_shifted_class.end() )
         {
            int idx = std::distance(row_shifted_class.begin(), iterator);
            // remove the entries from rows and rows shifted
            auto its = rows_shifted.begin();
            std::advance(its, idx);
            rows_shifted.erase(its);
            auto it = rows.begin();
            std::advance(it, idx);
            rows.erase(it);
         }
      }
   }
   return classes;
}

template <typename  Integer>
std::set<std::vector<double>> panda::algorithm::affineTransformation(const std::set<Row<Integer>>& input)
{
   // define transformed lists
   std::set<std::vector<double>> transformed;
   for ( auto& row: input )
   {
      // create a double vector
      std::vector<double> doubleVec;
      transform(row.begin(), row.end(), doubleVec.begin(), [](int x) { return (double)x;});
      // find the minimum value of the row and add it to all elements
      auto& min_value = *std::min_element(row.begin(), row.end());
      transform(doubleVec.begin(), doubleVec.end(), doubleVec.begin(), bind2nd(std::plus<double>(), min_value));
      // find the second smallest value using partial sort
      double b[2];
      std::partial_sort_copy(doubleVec.begin(), doubleVec.end(),b, b + 2);
      const auto& sec_min_value = b[1];
      // check that lowest value is zero
      assert ( b[0] == 0);
      // create a double vector
      // transform the double vector
      // TODO: This is not yet correct, we also need the deterministic points or at least the sum of those
      for (auto& val : doubleVec)
      {
         val = (val - (double) min_value) / (double)sec_min_value;
      }
      // add the double vector to the transformed matrix
      transformed.insert(transformed.end(), doubleVec);
   }
   return transformed;
}
template <typename Integer>
int checkEquivalence(std::set<std::vector<double>> rows_shifted, std::vector<double> row_shifted)
{
   // perform affine transformation on the single vectors
   return 1;
   //
}