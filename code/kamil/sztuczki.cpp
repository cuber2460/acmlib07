#include<ext/pb_ds/assoc_container.hpp> // ordered set
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds; template <typename T> using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
ordered_set<int> s; s.insert(1); s.insert(2);
 s.order_of_key(1);   // Out: 0.
*s.find_by_order(1);  // Out: 2.
// unordered_map hash.
struct MyHash { std::size_t operator()(const MojTyp& x) { /* ... */ } };
bool operator==(const MojTyp& a, const MojType& b) { /* ... */ }
std::unordered_set<MojTyp, MyHash> secik;
// Find first / find next on bitset.
for (int pos = bs._Find_first(); pos != bs.size(); pos = bs._Find_next(pos))
  { /* Something with @pos. */ }
// Ratio liczba_elementów / liczba bucketów, domyślnie 1.0 (ustaw na INF, jeśli
secik.max_load_factor(0.25);  // chcesz używać tylko reserve).
// Maksuje liczbę bucketów do danej liczby, zalecana potęga 2,
secik.reserve(1<<15);  // np. (1 << 22) dla n = 1e6.
int secik.bucket_count(); // Zwraca obecna liczbe bucketow, sluzy do testowania.
// limit_denominator
import fractions
a = fractions.Fraction(3.141592).limit_denominator(7)  // a = Fraction(22, 7)
print(a.numerator, a.denominator)
