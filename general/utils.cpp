#include "utils.h"
#include <cmath>

namespace helfem {
  namespace utils {
    double arcosh(double x) {
      return log(x+sqrt(x*x-1.0));
    }

    double arsinh(double x) {
      return log(x+sqrt(x*x+1.0));
    }

    arma::mat product_tei(const arma::mat & ijint, const arma::mat & klint) {
      const size_t Ni(ijint.n_rows);
      const size_t Nj(ijint.n_cols);
      const size_t Nk(klint.n_rows);
      const size_t Nl(klint.n_cols);

      arma::mat teiblock(Ni*Nj,Nk*Nl);
      teiblock.zeros();

      // Form block
      for(size_t fk=0;fk<Nk;fk++)
        for(size_t fl=0;fl<Nl;fl++) {
          // Use temp variable
          double kl(klint(fk,fl));

          for(size_t fi=0;fi<Ni;fi++)
            for(size_t fj=0;fj<Nj;fj++)
              // (ij|kl) in Armadillo compatible indexing
              teiblock(fj*Ni+fi,fl*Nk+fk)=kl*ijint(fi,fj);
        }

      return teiblock;
    }

    arma::mat exchange_tei(const arma::mat & tei, size_t Ni, size_t Nj, size_t Nk, size_t Nl) {
#ifndef ARMA_NO_DEBUG
      if(tei.n_rows != Ni*Nj) {
        std::ostringstream oss;
        oss << "Invalid input tei: was supposed to get " << Ni*Nj << " rows but got " << tei.n_rows << "!\n";
        throw std::logic_error(oss.str());
      }
      if(tei.n_cols != Nk*Nl) {
        std::ostringstream oss;
        oss << "Invalid input tei: was supposed to get " << Nk*Nl << " cols but got " << tei.n_cols << "!\n";
        throw std::logic_error(oss.str());
      }
#endif

      arma::mat ktei(Nj*Nk,Ni*Nl);
      ktei.zeros();
      for(size_t ii=0;ii<Ni;ii++)
        for(size_t jj=0;jj<Nj;jj++)
          for(size_t kk=0;kk<Nk;kk++)
            for(size_t ll=0;ll<Nl;ll++)
              // (ik|jl) in Armadillo compatible indexing
              ktei(kk*Nj+jj,ll*Ni+ii)=tei(jj*Ni+ii,ll*Nk+kk);

      return ktei;
    }
  }
}