int calc_percentage(int sent, int received, int total) {
    int res = 0;

    res = sent - received;

    return (res / total) * 100;
}
