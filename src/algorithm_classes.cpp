
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
#include "algorithm_matrix_operations.h"

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
Matrix<Integer> panda::algorithm::classesDeterministic(Matrix<Integer> input, const Maps& maps, Matrix<Integer> dets, TagType tag)
{
   std::set<Row<Integer>> rows(input.cbegin(), input.cend());
   return classesDeterministic(rows, maps, dets, tag);
}

template <typename Integer, typename TagType>
Matrix<Integer> panda::algorithm::classes(std::set<Row<Integer>> rows, const Maps& maps, TagType tag)
{
   Matrix<Integer> classes;
   while ( !rows.empty() )
   {
      const auto row_class = getClass(*rows.begin(), maps, tag);
      assert( !row_class.empty() );
      classes.push_back(*row_class.crbegin()); // Important detail: last element is chosen as the representative
      for ( const auto& row : row_class )
      {
         const auto position = rows.find(row);
         if ( position != rows.end() )
         {
            rows.erase(position);
         }
      }
   }
   return classes;
}

template <typename Integer, typename TagType>
Matrix<Integer> panda::algorithm::classesDeterministic(std::set<Row<Integer>> rows, const Maps& maps, Matrix<Integer> dets, TagType tag)
{
   // check if any deterministics are given -> otherwise let the usual algorithm run
   if ( dets.size() < 1)
   {
      return classes(rows, maps, tag);
   }
   std::cerr << "running deterministic version of classes algorithm \n";
   std::cerr << "Number of deterministics: " << dets.size() << "\n";

   // class representatives to return
   Matrix<Integer> classes_repr;
   while ( !rows.empty() )
   {
      std::cerr << "rows.size(): " << rows.size() << "\n";
      // Get the class of the first row
      const auto rows_class = getClass(*rows.begin(), maps, tag);
      assert ( !rows_class.empty() );
      assert ( panda::algorithm::checkEquivalence(*rows_class.begin(), *rows.begin(), dets));
      // add the representative of the class to the return matrix
      classes_repr.push_back(*rows_class.crbegin());
      // iterate over the rows of that class
      for ( auto row_class : rows_class )
      {
         // generate a copy of rows (needed to run proper for-loop)
         std::set<Row<Integer>> copy_rows (rows);
         // check if any row in rows is equivalent to rows_class
         for ( const auto& row : copy_rows)
         {
            // Test if both rows are equivalent
            if ( panda::algorithm::checkEquivalence(row_class, row, dets) )
            {
               // if equiv -> delete that row
               const auto position = rows.find(row);
               rows.erase(position);
            }

         }
      }
   }
   return classes_repr;
}

template <typename  Integer>
std::set<std::vector<double>> panda::algorithm::affineTransformation(const std::set<Row<Integer>>& input)
{
   // define transformed lists
   std::set<std::vector<double>> transformed;
   for ( auto row: input)
   {
      // create a double vector from the row
      std::vector<double> doubleVec(row.begin(), row.end());
      // find the two smallest values
      double b[2];
      std::partial_sort_copy(doubleVec.begin(), doubleVec.end(), b, b + 2);
      // transform the double vector
      for (int i = 0; i < doubleVec.size(); i++)
      {
         double val = (doubleVec[i] - b[0]) / b[1];
         // round the vector
         val = round( val * 1000.0 ) / 1000.0;
         doubleVec[i] = val;
      }
      transformed.insert(transformed.end(), doubleVec);
   }
   return transformed;
}

template <typename  Integer>
bool panda::algorithm::checkEquivalence(const Row<Integer>& row_one, const Row<Integer>& row_two, const Deterministics<Integer>& dets)
{
   assert ( row_one.size() == row_two.size() );
   // multiply the vectors by the deterministics
   Row<Integer> v_one = dets * row_one;
   Row<Integer> v_two = dets * row_two;
   //row_two = dets * row_two;
   // Generate double vectors from the integer input vectors
   std::vector<double> d_one(v_one.begin(), v_one.end());
   std::vector<double> d_two(v_two.begin(), v_two.end());
   // find the two lowest values in each vector
   double low_v_one[2];
   double low_v_two[2];
   std::partial_sort_copy(d_one.begin(), d_one.end(), low_v_one, low_v_one + 2);
   std::partial_sort_copy(d_two.begin(), d_two.end(), low_v_two, low_v_two + 2);
   // rescale the two vectors
   for ( int i = 0; i < d_one.size(); i++)
   {
      d_one[i] = ( d_one[i] - low_v_one[0] ) / low_v_one[1];
      d_two[i] = ( d_two[i] - low_v_two[0] ) / low_v_two[1];
      // round the values
      d_one[i] = round(d_one[i] * 1000) / 1000;
      d_two[i] = round(d_two[i] * 1000) / 1000;
   }
   // check if the two vectors are equal
   return d_one == d_two;
}