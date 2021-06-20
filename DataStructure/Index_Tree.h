#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;

typedef tree<int,null_type,less<int>,rb_tree_tag,tree_order_statistics_node_update> IndexTree;

// Ex;
s={2,5,6,10}
*s.find_by_order(2)=6;
s.order_of_key(x) == lower_bound(x)-s.begin();
