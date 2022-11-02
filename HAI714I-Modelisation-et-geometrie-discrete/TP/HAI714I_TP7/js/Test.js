class Test {
  static defaultAction = "draw";
  cp; // control panel
  a; // action
  d; // displayer
  centre = new Coord2D(0, 0);
  referencePoint;
  states = { center: 2, comp: 3, smaller: -1, bigger: 1, equal: 0 };

  constructor(_action) {
    this.a = _action;
  }

  //priori simplicité (version draft: les clés des options ui sont utilisées telles quelles dans un if else infernal, beurk mais rapide, à revoir ?)
  //ui keys = tour, comp, tri, algo_dp, algo_jarvis, algo_graham
  run() {
    this.d.init();
    this.d.setModelSpace(...this.getModelSpace());
    this.d.drawPoints(this.getPoints(), false);

    //todo
    //let key = "algo_dp";
    //let key = "tour";
    //let key = "comp";
    let key = "algo_graham";
    const t0 = performance.now();
    let p = this.getPoints();
    if (key == "tour") {
      let n = this.getPoints().length;
      console.log(n);
      for (let i = 0; i < n - 1; i++) {
        for (let j = i; j < n; j++) {
          for (let k = i; k < n; k++) {
            if (k != j) {
              this.d.drawTour(
                Coord2D.tour(p[i], p[j], p[k]),
                [p[i], p[j], p[k]],
                true
              );
            }
          }
        }
      }
      /* 
      let n = this.getPoints().length;
      let i;
      for (i = 0; i < n - 2; i += 3) {
        this.d.drawTour(
          Coord2D.tour(p[i], p[i + 1], p[i + 2]),
          [p[i], p[i + 1], p[i + 2]],
          true
        );
      }
      if (n != i) {
        // Si on a un nombre non multiple de 3 de points il faut tracer ce qu'il reste a partir de points déja use
        this.d.drawTour(
          Coord2D.tour(p[n - 3], p[n - 2], p[n - 1]),
          [p[n - 3], p[n - 2], p[n - 1]],
          true
        );
      }*/
      const t1 = performance.now();
      console.log(
        `Tour en O(n3) a pris ${t1 - t0} milliseconds pour ${
          this.getPoints().length
        } elements.`
      );
    }

    if (key == "algo_dp") {
      let alg_dp = new EnveloppeConvexe();
      this.d.drawLineLoop(alg_dp.algoDemiPlan(p), true);
      //algoDemiPlan(this.getPoints());
    }

    if (key == "comp") {
      this.d.drawPointsComp(p, true);
    }

    if (key == "algo_graham") {
      let test = new EnveloppeConvexe(this.getPoints(), 2);
      //console.log("Graham = " + test.algo_graham(this.getPoints()))
      console.log("Graham = " + test.envconv);
      for (let i = 0; i < test.envconv.length - 1; i++) {
        this.d.mDrawLine(test.envconv[i], test.envconv[i + 1]);
      }
      const t1 = performance.now();
      console.log(
        `Graham a pris ${t1 - t0} milliseconds pour ${
          this.getPoints().length
        } elements.`
      );
    }
  }

  setUI(_controlPanel, _displayer) {
    this.cp = _controlPanel;
    this.d = _displayer;
  }

  changeAction(_newA) {
    this.a = _newA;
    this.run();
  }

  refresh() {
    this.run();
  }

  // made to fit the signature of setModelSpace@Displayer(minX, minY, maxX, maxY)
  getModelSpace() {
    return [
      this.cp.getDataSet().minX,
      this.cp.getDataSet().minY,
      this.cp.getDataSet().maxX,
      this.cp.getDataSet().maxY,
    ];
  }

  getPoints() {
    return this.cp.getDataSet().getPoints();
  }

  renewData() {
    this.cp.renewData();
    this.refresh();
  }
}
