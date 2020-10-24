var gulp = require("gulp");
var sass = require("gulp-sass");
var autoprefixer = require("autoprefixer");
var sourcemaps = require("gulp-sourcemaps");
var postcss = require("gulp-postcss");
var babel = require("gulp-babel");

gulp.task("sass", function () {
  return gulp
    .src("./styles.scss")
    .pipe(sourcemaps.init())
    .pipe(sass())
    .pipe(postcss([autoprefixer()]))
    .pipe(sourcemaps.write("."))
    .pipe(gulp.dest("./dist"));
});

gulp.task("sass:watch", function () {
  gulp.watch("./styles.scss", gulp.series("sass"));
});

gulp.task("js", function () {
  return gulp
    .src("./main.js")
    .pipe(sourcemaps.init())
    .pipe(babel())
    .pipe(sourcemaps.write("."))
    .pipe(gulp.dest("./dist"));
});

gulp.task("js:watch", function () {
  gulp.watch("./main.js", gulp.series("js"));
});

gulp.task("all:watch", function () {
  gulp.watch(["styles.scss", "./main.js"], gulp.series("sass", "js"));
});
