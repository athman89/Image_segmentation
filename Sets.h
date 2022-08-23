#ifndef SEGMENTATION_SETS_H
#define SEGMENTATION_SETS_H
#ifndef DISJOINT_SET
#define DISJOINT_SET

/*Using Union-find algorithm to create a forest of disjoint-set*/

typedef struct {
    int p;
    int rank;
    int sz;
} elt;

class universe {
public:
    universe(int elements);
    ~universe();
    int find(int x);
    void join(int x, int y);
    int size(int x) const { return elts[x].sz; }
    int num_sets() const { return num; }

private:
    elt *elts;
    int num;
};

universe::universe(int elem) {
    elts = new elt[elem];
    num = elem;
    for (int i = 0; i < elem; i++) {
        elts[i].rank = 0;
        elts[i].sz = 1;
        elts[i].p = i;
    }
}

universe::~universe() {
    delete [] elts;
}

int universe::find(int x) {
    int y = x;
    while (y != elts[y].p)
        y = elts[y].p;
    elts[x].p = y;
    return y;
}

void universe::join(int x, int y) {
    if (elts[x].rank > elts[y].rank) {
        elts[y].p = x;
        elts[x].sz += elts[y].sz;
    } else {
        elts[x].p = y;
        elts[y].sz += elts[x].sz;
        if (elts[x].rank == elts[y].rank)
            elts[y].rank++;
    }
    num--;
}

#endif

#endif //SEGMENTATION_SETS_H
