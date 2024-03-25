/// Authors: 
/// Vadim Goncearenco (xgonce00@stud.fit.vutbr.cz)
/// 
class Film {
  final int? id;

  String title = "<No title>";
  String overview = "<No overview>";
  String posterPath = "<No poster>";
  String actors = "<No actors>";
  String releaseDate = "<No release date>";
  String director = "<No director>";
  String duration = "<No duration>";
  Film() : id = null;

  Film.fromMap(Map<String, dynamic> map)
      : id = map['id'] as int,
        title = map['title'] as String,

        releaseDate = map['releaseDate'] as String,
        duration = map['duration'] as String,
        posterPath = map['posterPath'] as String,
        director = map['director'] as String,
        overview = map['overview'] as String,
        actors = map['actors'] as String
  ;

  Map<String, dynamic> toMap() {
    final map = {
      'title': title,
      'releaseDate': releaseDate,
      'duration': duration,
      'posterPath': posterPath,
      'director': director,
      'overview': overview,
      'actors': actors
    };

    if (id != null) {
      map['id'] = id!.toString();
    }

    return map;
  }

  @override
  String toString() {
    return 'Film{id: $id, title: $title}';
  }
}
