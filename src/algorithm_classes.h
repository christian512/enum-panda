
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <set>

#include "maps.h"
#include "matrix.h"
#include "row.h"
#include "tags.h"

namespace panda
{
   namespace algorithm
   {
      /// returns the unique class representative. The class is generated with the provided maps.
      /// Precondition: if input is a facet, the facet must be normalized.
      template <typename Integer, typename TagType>
      Row<Integer> classRepresentative(const Row<Integer>&, const Maps&, TagType);
      /// Creates a set of rows which is the complete class containing the input row.
      /// Precondition: if input is a facet, the facet must be normalized.
      template <typename Integer, typename TagType>
      std::set<Row<Integer>> getClass(const Row<Integer>&, const Maps&, TagType);
      /// Reduces a list of rows to just the representatives.
      /// Precondition: if input are facets, then these facets must be normalized.
      template <typename Integer, typename TagType>
      Matrix<Integer> classes(Matrix<Integer>, const Maps&, TagType);
      /// Reduces a list of rows to just the representative using deterministic points.
      /// Precondition: deterministics must be given
      template <typename Integer, typename TagType>
      Matrix<Integer> classesDeterministic(Matrix<Integer>, const Maps&, Matrix<Integer>, TagType);
      /// Reduces a list of rows to just the representatives.
      /// Precondition: if input are facets, then these facets must be normalized.
      template <typename Integer, typename TagType>
      Matrix<Integer> classes(std::set<Row<Integer>>, const Maps&, TagType);
      /// Reduces a list of rows to just the representative using deterministic points.
      /// Precondition: deterministics must be given
      template <typename Integer, typename TagType>
      Matrix<Integer> classesDeterministic(std::set<Row<Integer>>, const Maps&, Matrix<Integer>, TagType);
      /// Transforms a list of rows under affine transformation
      /// Precondition: None?
      template <typename Integer>
      std::set<std::vector<double>> affineTransformation(const std::set<Row<Integer>>&);
      /// Checks if two given vertices are equivalent
      /// Precondition: first input is list of shifted rows, second is the one to check for equivalence
      int checkEquivalence(std::set<std::vector<double>>, std::vector<double>);
   }
}

#include "algorithm_classes.eti"

