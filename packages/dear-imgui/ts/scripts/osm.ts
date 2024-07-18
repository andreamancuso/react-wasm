import { readFileSync } from "fs";
import osmtogeojson from "osmtogeojson";

const maroggiaOsm = readFileSync("./scripts/maroggia.osm");

const osm = osmtogeojson(maroggiaOsm.toString());

console.log(osm);
