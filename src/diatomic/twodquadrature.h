/*
 *                This source code is part of
 *
 *                          HelFEM
 *                             -
 * Finite element methods for electronic structure calculations on small systems
 *
 * Written by Susi Lehtola, 2018-
 * Copyright (c) 2018- Susi Lehtola
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#ifndef DIATOMIC_2DQUAD_H
#define DIATOMIC_2DQUAD_H

#include "basis.h"
#include "../general/sap.h"

namespace helfem {
  namespace diatomic {
    namespace twodquad {
      /// Where to place probe?
      typedef enum {
                    // Probe left atom
                    PROBE_LEFT,
                    // Probe midbond
                    PROBE_MIDDLE,
                    // Probe right atom
                    PROBE_RIGHT
      } probe_t;

      /// Worker class
      class TwoDGridWorker {
      protected:
        /// Basis set
        const helfem::diatomic::basis::TwoDBasis *basp;

        /// Angular grid
        arma::vec cth, wang;
        /// Radial grid
        arma::vec r;
        /// Radial weight
        arma::rowvec wrad;
        /// Total quadrature weight
        arma::rowvec wtot;

        /// Value of m
        int m;
        /// List of basis functions in element
        arma::uvec bf_ind;
        /// Values of important functions in grid points, Nbf * Ngrid
        arma::mat bf;

        /// Value of integrand, Ngrid
        arma::mat itg;

      public:
        /// Dummy constructor
        TwoDGridWorker();
        /// Constructor
        TwoDGridWorker(const helfem::diatomic::basis::TwoDBasis * basp, int lang);
        /// Destructor
        ~TwoDGridWorker();

        /// Compute basis functions on grid points
        void compute_bf(size_t iel, size_t irad, int m);
        /// Free memory
        void free();

        /// Compute SAP potential
        void sap_pot(const ::SAP & sap, int Z1, int Z2);
        /// Compute GSZ potential
        void gsz_pot(int Z1, double d1, double H1, int Z2, double d2, double H2);
        /// Set unit potential
        void unit_pot();

        /// Compute GTO projection
        void gto(int l, const arma::vec & expn, probe_t p);
        /// Compute STO projection
        void sto(int l, const arma::vec & expn, probe_t p);

        /// Evaluate potential energy matrix elements (for GSZ and SAP)
        void eval_pot(arma::mat & V) const;
        /// Evaluate basis set projection
        void eval_proj(arma::mat & S) const;
        /// Evaluate projection's overlap
        void eval_proj_overlap(arma::mat & S) const;
      };

      /// Wrapper routine
      class TwoDGrid {
      private:
        /// Pointer to basis set
        const helfem::diatomic::basis::TwoDBasis * basp;
        /// Angular rule
        int lang;

      public:
        /// Dummy constructor
        TwoDGrid();
        /// Constructor
        TwoDGrid(const helfem::diatomic::basis::TwoDBasis * basp, int lang);
        /// Destructor
        ~TwoDGrid();

        /// Compute GSZ matrix
        arma::mat GSZ(int Z1, double d1, double H1, int Z2, double d2, double H2);
        /// Compute GSZ matrix with default parameters
        arma::mat GSZ();
        /// Compute SAP matrix
        arma::mat SAP(const ::SAP & sap);

        /// Compute overlap matrix
        arma::mat overlap();
        /// Compute GTO projection
        arma::mat gto_projection(int l, int m, const arma::vec & expn, probe_t p);
        /// Compute GTO projection
        arma::mat gto_overlap(int l, int m, const arma::vec & expn, probe_t p);
        /// Compute STO projection
        arma::mat sto_projection(int l, int m, const arma::vec & expn, probe_t p);
        /// Compute STO overlap
        arma::mat sto_overlap(int l, int m, const arma::vec & expn, probe_t p);
      };
    }
  }
}

#endif