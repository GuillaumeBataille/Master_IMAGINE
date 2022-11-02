class EnveloppeConvexe {
  static algo = { demiPlan: 0, jarvis: 1, graham: 2 };
  constructor(lesPoints, a) {
    this.algo = a ?? EnveloppeConvexe.algo.jarvis;
    this.points = lesPoints;
    this.envConv = new Array();
    this.runAlgo(this.algo);
  }

  getEnvConv() {
    return this.envConv;
  }

  updateEnvConvFromIndices(indices) {
    this.envConv = new Array();
    for (let i = 0; i < indices.length; i++) {
      this.envConv.push(this.points[indices[i]]);
    }
  }

  runAlgo(idAlgo) {
    this.algo = idAlgo;
    switch (this.algo) {
      case EnveloppeConvexe.algo.demiPlan:
        this.envconv = this.algoDemiPlan(this.points);
        break;
      case EnveloppeConvexe.algo.jarvis:
        this.envconv = this.algoJarvis(this.points);
        break;
      case EnveloppeConvexe.algo.graham:
        this.envconv = this.algoGraham(this.points);
        break;
      default:
        console.log("algo non défini => algo jarvis utilisé");
        this.envconv = this.algoJarvis(this.points);
        break;
    }
  }

  findNextIdx(currentIdx, points) {
    //todo: fonction utile pour l'algo de Jarvis
  }

  findFirst(points, i, j) {
    let n = points.length;
    let res;
    for (let k = 0; k < n; k++) {
      if (points[i] != points[k] && points[j] != points[k]) return res;
    }
  }

  algoDemiPlan(points) {
    result;
    let V = points;
    let n = V.length;
    let current, previous, i, j, k, f;
    let mc;
    // Debut
    for (i = 1; i < n - 1; i++) {
      for (j = 0; j < i; j++) {
        mc = true;
        f = this.findFirst(V, i, j);
        previous = Coord2D.tour(V[i], V[j], V[f]);
        k = f + 1;
        do {
          if (k != i && k != j) {
            current = tour(V[i], V[j], V[k]);
            if (current == 0) {
              return -1; //Exeption
            } else if (previous != current) {
              mc = false;
            }
          }
          previous = current;
        } while (++k < n && mc);

        if (k == n && previous == current) {
          if (current > 0) result.add;
        }
      }
    }
    return result;
  }

  algoJarvis(points) {
    //todo: implementation de cet algo
  }

  tri(min, points) {
    let triRadial = new TriRadial(points, min);
    return triRadial.V;
  }
  algoGraham(points) {
    //todo: implementation de cet algo
    // Initialisation
    let min = Coord2D.findMinIdx(points); // Recup l'indice du minY

    let points_copy = points.map((x) => x); // Copie de points
    points_copy.splice(min, 1); // Retire le min de l'ensemble

    let sorted_points = this.tri(points[min], points_copy); // On trie par rapport a min
    let candidates = sorted_points.map((x) => x); // Recopier les points triés
    candidates.push(points[min]); // Ajout de min en fin de liste

    let result = [points[min], candidates[0]];
    let n = candidates.length;
    let m = result.length;

    for (let k = 1; k <= n - 1; k++) {
      let t;
      while (
        m >= 2 &&
        (t = Coord2D.tour(result[m - 2], result[m - 1], candidates[k])) <= 0
      ) {
        if (t == 0) {
          new Error("alignment", result[m - 2], result[m - 1], candidates[k]);
        }
        result.pop();
        m = result.length;
      }
      result.push(candidates[k]);
      m = result.length;
    }
    return result;
  }
}
