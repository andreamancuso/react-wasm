import { readFileSync } from "fs";
// @ts-ignore
import Pbf from "pbf";
// @ts-ignore
import { VectorTile } from "@mapbox/vector-tile";

const testPbf = readFileSync("./scripts/test.pbf");

const pbf = new Pbf(testPbf);
const vectorTile = new VectorTile(pbf);

console.log(vectorTile.layers.admin.length);

for (let i = 0; i < vectorTile.layers.admin.length; i++) {
    const feature = vectorTile.layers.admin.feature(i);

    console.log(feature.type);

    if (feature.type === 3) {
        console.log(feature.toGeoJSON(0, 0, 0));
        break;
    }
}
