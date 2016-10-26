/* FFT dla doubli O(n*log(n)).                                                */
/* Wynik mnożenia dwóch wielomianów.                                          */
/* Argumenty to dwa wektory, wynik to jeden wektor.                           */
/* fft() działa bezpiecznie dla liczb do 35 * 1000.  Dla większych odpalamy   */
/* fft_dokladne(), które działa wolniej, ale liczy dokładnie.                 */
/* Można je zmodyfikować do liczenia modulo np. 10^9 + 7                      */
/* Sprawdzić, czy argumenty nie powinny być wektorami long longów.            */

// TODO(Mateusz): "Argumenty to dwa wektory, wynik to jeden wektor" <- opisać
// strukturę wektorów, (np. długości, w jakich indeksach są wartości, itd.).
// TODO(Mateusz): Zrobić template, żeby było prosto się przełączyć na
// long double.
struct fft_double {
  double PI = 3.141592653589793;
  using Complex = complex<double>;

  // TODO(Mateusz): Po co pisać, że potrzebne? To chyba powinno być domyślne.
  vector<Complex> dftn;  // Potrzebne.
  vector<Complex> dfts;  // Potrzebne.

  // TODO(Mateusz): Do zwykłego czego?  Jest trochę miejsca wolnego w tej linii
  // na komentarz, to można śmiało się rozpisać.
  vector<Complex> pom;  // Do zwykłego.

  vector<Complex> a1;  // Do dokładnego.
  vector<Complex> a2;  // Do dokładnego.
  vector<Complex> b1;  // Do dokładnego.
  vector<Complex> b2;  // Do dokładnego.

  vector<double> cosi;  // Potrzebne.
  vector<Complex> omega;  // Potrzebne.

  fft_double() {}

  // @n to maksymalna suma rozmiarów mnożonych wielomianów.
  // TODO(Mateusz): @chce_dokladne to nie powinien być bool?
  fft_double(int n, int chce_dokladne) {
    n = potenga(n);
    dftn.resize(n + 1, 0);
    dfts.resize(n + 1, 0);
    pom.resize(n + 1, 0);
    if (chce_dokladne) {
      a1.resize(n + 1, 0);
      a2.resize(n + 1, 0);
      b1.resize(n + 1, 0);
      b2.resize(n + 1, 0);
    }
    cosi.resize(n + 1, 0);
    omega.resize(n + 1, 0);
  }

  // TODO(Mateusz): Pisać normalne słowa, nie "potenga".
  inline int potenga(int v) {
    // TODO(Mateusz): Nie pisać "<<=", jeśli to jest mnożenie przez 2.
    // TODO(Mateusz): Nie pisać "1" zamiast "true".
    // TODO(Mateusz): W pętli "for" można ewentualnie zostawić pustą środkową
    // część.  Wtedy to jest równoważne "true" lub "1".
    for (int i = 1; 1; i <<= 1) {
      if (i >= v) {
        return i;
      }
    }
    // TODO(Mateusz): Dopisać tutaj "abort();  // Unreachable code.".
  }

  inline void dft(int n, int kier) {
    int n2 = n - 1;
    int s = 0;
    int p;
    int g;
    // TODO(Mateusz): Jeśli "<<=" logicznie ma odpowiadać mnożeniu przez 2,
    // to nie należy tak pisać.
    for (int i = 2; i <= n; i <<= 1) {
      dftn.swap(dfts);
      p = n / i;
      if (kier) {
        g = 0;
        for (int j = 0; j < n; j++) {
          // TODO(Mateusz): To wyrażenie jest zbyt skomplikowane, powinno być
          // rozbite na kilka części.  Prościej wtedy czytać (i przepisywać!).
          dftn[j] = dfts[(2 * (j - s) + s) & n2] +
                    omega[g] * dfts[(2 * (j - s) + p + s) & n2];
          s++;
          if (s == p) {
            g = j + 1;
            s = 0;
          }
        }
      } else {
        g = n;
        for (int j = 0; j < n; j++) {
          // TODO(Mateusz): Jak się pisze kod na kontestach, to można zrobić
          // copy-paste, żeby zaoszczędzić czas.  Ale jak się robi biblioteczkę
          // to trzeba się postarać, żeby kopii nie było!
          dftn[j] = dfts[(2 * (j - s) + s) & n2] +
                    omega[g] * dfts[(2 * (j - s) + p + s) & n2];
          s++;
          if (s == p) {
            g = n - j - 1;
            s = 0;
          }
        }
      }
    }
  }

  void licz_omegi(int n1) {
    double kat = 2.0 * PI / n1;
    int n2 = n1 - 1;
    int dod = 3 * n1 / 4;
    for (int i = 0; i <= n1; i++)
      cosi[i] = cos(kat * i);
    for (int i = 0; i <= n1; i++)
      omega[i] = Complex(cosi[i], cosi[(i + dod) & n2]);
  }

  vector<int> fft(vector<int> &jed, vector<int> &dwa) {
    int n1 = potenga(jed.size() + dwa.size());
    licz_omegi(n1);
    for (int i = 0; i < (int) jed.size(); i++)
      dftn[i] = jed[i];
    for (int i = (int) jed.size(); i < n1; i++)
      dftn[i] = 0;
    dft(n1, 1);
    for (int i = 0; i < n1; i++)
      pom[i] = dftn[i];
    for (int i = 0; i < (int) dwa.size(); i++)
      dftn[i] = dwa[i];
    for (int i = (int) dwa.size(); i < n1; i++)
      dftn[i] = 0;
    dft(n1, 1);
    for (int i = 0; i < n1; i++)
      dftn[i] *= pom[i];
    dft(n1, 0);
    vector<int> ret;
    for (int i = 0; i < n1; i++)
      ret.push_back(llround(dftn[i].real() / n1));
    return ret;
  }

  vector<int> fft_dokladne(vector<int> &jed, vector<int> &dwa) {
    int n1 = potenga(jed.size() + dwa.size());
    licz_omegi(n1);
    // TODO(Mateusz): Co to za stała?
    long long M = 32000;
    for (int i = 0; i < jed.size(); i++)
      dftn[i] = jed[i] / M;
    for (int i = jed.size(); i < n1; i++)
      dftn[i] = 0;
    dft(n1, 1);
    for (int i = 0; i < n1; i++)
      a1[i] = dftn[i];

    for (int i = 0; i < jed.size(); i++)
      dftn[i] = jed[i] % M;
    for (int i = (int) jed.size(); i < n1; i++)
      dftn[i] = 0;
    dft(n1, 1);
    for (int i = 0; i < n1; i++)
      b1[i] = dftn[i];
    for (int i = 0; i < (int) dwa.size(); i++)
      dftn[i] = dwa[i] / M;
    for (int i = (int) dwa.size(); i < n1; i++)
      dftn[i] = 0;
    dft(n1, 1);
    for (int i = 0; i < n1; i++)
      a2[i] = dftn[i];
    for (int i = 0; i < (int) dwa.size(); i++)
      dftn[i] = dwa[i] % M;
    for (int i = (int) dwa.size(); i < n1; i++)
      dftn[i] = 0;
    dft(n1, 1);
    for (int i = 0; i < n1; i++)
    b2[i] = dftn[i];

    vector<int> ret;
    for (int i = 0; i < n1; i++)
    ret.push_back(0);
    for (int i = 0; i < n1; i++)
      dftn[i] = a1[i] * a2[i];
    dft(n1, 0);
    for (int i = 0; i < n1; i++)
      ret[i] += llround(dftn[i].real() / n1) * M * M;
    for (int i = 0; i < n1; i++)
      dftn[i] = a1[i] * b2[i] + b1[i] * a2[i];
    dft(n1, 0);
    for (int i = 0; i < n1; i++)
      ret[i] += llround(dftn[i].real() / n1) * M;
    for (int i = 0; i < n1; i++)
      dftn[i] = b1[i] * b2[i];
    dft(n1, 0);
    for (int i = 0; i < n1; i++)
      ret[i] += llround(dftn[i].real() / n1);
    return ret;
  }
};
