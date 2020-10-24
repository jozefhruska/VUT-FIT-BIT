var gulp = require("gulp");
var sass = require("gulp-sass");

gulp.task("sass", function() {
  return gulp
    .src("./itw1.scss")
    .pipe(sass())
    .pipe(gulp.dest("."));
});

gulp.task("watch", function() {
  gulp.watch("./itw1.scss", gulp.series('sass'));
});
