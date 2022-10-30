struct Position
{
    int r; // ligne;
    int c; // colonne;

    bool operator==(const Position x){
        return (this->r == x.r)  && (this->c == x.c);
    }

    bool operator!=(const Position x){
        return !operator==(x);
    }
};



Position add(Position a , Position b){ // Ajout de deux position
    Position res;
    res.r = a.r + b.r;
    res.c = a.c + b.c;
    return res;
};