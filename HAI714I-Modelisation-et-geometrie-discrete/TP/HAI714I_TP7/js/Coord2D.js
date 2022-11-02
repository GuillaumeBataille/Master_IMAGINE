class Coord2D {
  constructor(x, y, label) {
    this.x = x;
    this.y = y;
    this.label = label;
  }
  getRounded(a) {
    if (Number.isInteger(a)) return a;
    else return Math.round(a * 100) / 100;
  }
  getRoundedX() {
    return this.getRounded(this.x);
  }
  getRoundedY() {
    return this.getRounded(this.y);
  }

  //a et b sont des points du plan représentés par leurs coordonnées en 2D
  static vecteur(a, b) {
    return new Coord2D(b.x - a.x, b.y - a.y, a.label + b.label);
  }

  //a et b sont des points du plan représentés par leurs coordonnées en 2D
  static middle(a, b, label) {
    return new Coord2D((b.x + a.x) / 2, (a.y + b.y) / 2, label ?? "m");
  }

  // v1 et v2 sont des vecteurs du plan représentés par leurs coordonnées en 2D
  static determinant(v1, v2) {
    // todo
    return v1.x * v2.y - v1.y * v2.x;
  }

  // v1 et v2 sont des vecteurs du plan représentés par leurs coordonnées en 2D
  static detSign(v1, v2) {
    //todo
    let det = this.determinant(v1, v2);
    return det / Math.abs(det);
  }

  static tour(o, p1, p2) {
    //todo
    let v1 = this.vecteur(o, p1);
    let v2 = this.vecteur(o, p2);

    return this.detSign(v1, v2);
  }

  // donne la position relative de p1 et de p2 dans un ordre trigonométrique, centré sur l'origine
  // les résultats de cette fonction sont inspirés du signe du sinus de l'angle entre les vecteurs op1 et op2:
  // 1 : lorsque p1 < p2 ( p1 *avant* p2 dans l'ordre trigonométrique sur [0, 2Pi[ )
  // -1 : lorsque p1 > p2 (p1 *après* p2 dans l'ordre trigonométrique sur [0, 2Pi[ )
  // 0 : lorsque O, p1, p2 sont alignés
  static compare(origine, p1, p2) {
    //todo
    // X est la base avec laquelle on va créer nos angles
    let vec_x = Coord2D.vecteur(new Coord2D(0, 0, "O"), new Coord2D(1, 0, "X"));

    let vec1 = Coord2D.vecteur(origine, p1);
    let vec2 = Coord2D.vecteur(origine, p2);

    let dot_prod1 = vec1.x * vec_x.x + vec1.y * vec_x.y;
    let len_vec1 = Math.sqrt(vec1.x ** 2 + vec1.y ** 2);
    let cos1 = dot_prod1 / len_vec1;

    let dot_prod2 = vec2.x * vec_x.x + vec2.y * vec_x.y;
    let len_vec2 = Math.sqrt(vec2.x ** 2 + vec2.y ** 2);

    let cos2 = dot_prod2 / len_vec2;

    let angle1 = Math.acos(cos1);
    let angle2 = Math.acos(cos2);

    if (angle1 < 0) angle1 += Math.PI * 2;
    if (angle2 < 0) angle2 += Math.PI * 2;

    if (angle1 < angle2) {
      return 1;
    } else if (angle2 < angle1) {
      return -1;
    } else {
      return 0;
    }
  }

  static findMinIdx(points) {
    //todo
    let current_minY = Number.MAX_SAFE_INTEGER;
    let current_min_id;

    for (var i = 0; i < points.length; i++) {
      if (points[i].y < current_minY) {
        current_minY = points[i].y;
        current_min_id = i;
      }
    }
    return current_min_id;
  }

  toString() {
    return (
      this.label + "(" + this.getRoundedX() + "," + this.getRoundedY() + ")"
    );
  }

  //utile au dessin des fleches
  rotate(alpha, centre) {
    let result = new Coord2D(0, 0, "image+(" + this.label + ")");
    let cos = Math.cos(alpha),
      sin = Math.sin(alpha);
    result.x =
      cos * this.x - sin * this.y - cos * centre.x + sin * centre.y + centre.x;
    result.y =
      sin * this.x + cos * this.y - sin * centre.x - cos * centre.y + centre.y;
    return result;
  }
}
