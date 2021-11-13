void simulateAnneal() {
    const double INIT_TEMP = 2e5;
    const double DELTA = 0.997;
    const double EPS = 1e-14;
    double curx = ansx, cury = ansy;
    for (double temp = INIT_TEMP; temp > EPS; temp *= DELTA) {
        double xx = curx + ((rand() << 1) - RAND_MAX) * temp;
        double yy = cury + ((rand() << 1) - RAND_MAX) * temp;
        double cure = calcEnergy(xx, yy);
        double diff = cure - anse;
        if (diff < 0) {
            ansx = curx = xx;
            ansy = cury = yy;
            anse = cure;
        } else if (exp(-diff / temp) * RAND_MAX > rand()) {
            curx = xx;
            cury = yy;
        }
    }
}
