/// Authors:
/// Tatiana Fedorova (xfedor14@stud.fit.vutbr.cz)
///
import 'package:flutter/material.dart';
import 'package:flutter_card_swiper/flutter_card_swiper.dart';
import 'package:itu_app/controller/user_controller.dart';
import 'package:itu_app/controller/user_films_controller.dart';
import 'package:itu_app/model/user_films_model.dart';
import 'package:provider/provider.dart';
import 'package:go_router/go_router.dart';

import 'package:itu_app/controller/film_controller.dart';
import 'package:itu_app/model/film_model.dart';

import 'package:itu_app/common/film_card_widgets.dart';

class GalleryScreen extends StatelessWidget {
  final int userId;
  const GalleryScreen({Key? key, required this.userId}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    var filmController = context.watch<FilmController>();

    return Scaffold(
      appBar: AppBar(
        title: const Text('Film selection'),
        backgroundColor: const Color.fromARGB(255, 68, 70, 115),
        foregroundColor: Colors.white,
      ),
      body: FutureBuilder(
        future: filmController.films(),
        builder: (context, snapshot) {
          if (snapshot.hasError) {
            return Text('Error: ${snapshot.error}');
          } else if (!snapshot.hasData) {
            return const CircularProgressIndicator();
          } else {
            List<Film> films = snapshot.data as List<Film>;

            if (films.isEmpty) {
              return const Center(
                child: Text('No films available.'),
              );
            }

            return SafeArea(
              child: FilmSwiper(films: films, userId: userId),
            );
          }
        },
      ),
    );
  }
}

class FilmSwiper extends StatefulWidget {
  final List<Film> films;
  final int userId;

  const FilmSwiper({Key? key, required this.films, required this.userId})
      : super(key: key);

  @override
  _FilmSwiperState createState() => _FilmSwiperState();
}

class _FilmSwiperState extends State<FilmSwiper> {
  final CardSwiperController cardSwiperController = CardSwiperController();
  int currentIndex = 0;
  bool isRoundCompleted = false;
  List<int> viewedFilmIds = [];
  String feedbackMessage = '';
  Color feedbackColor = Colors.black;
  String name = '';

  @override
  void initState() {
    super.initState();
    fetchViewedFilms();
    fetchUserName();
  }

  Future<void> fetchViewedFilms() async {
    List<UserFilms> viewedFilms =
        await Provider.of<UserFilmsController>(context, listen: false)
            .userFilms(widget.userId);
    setState(() {
      viewedFilmIds = viewedFilms.map((film) => film.filmId).toList();
    });
  }

  Future<void> fetchUserName() async {
    String userName = await Provider.of<UserController>(
      context,
      listen: false,
    ).getUserName(widget.userId);

    name = userName;
  }

  @override
  Widget build(BuildContext context) {
    List<Film> filteredFilms =
        widget.films.where((film) => !viewedFilmIds.contains(film.id)).toList();

    return Stack(
      alignment: Alignment.center,
      children: [
        if (!isRoundCompleted && filteredFilms.isNotEmpty)
          Column(
            children: [
              const SizedBox(height: 10),
              Text(
                '$name\'s turn!',
                style: const TextStyle(
                  fontSize: 20,
                  fontWeight: FontWeight.bold,
                ),
              ),
              const SizedBox(height: 20),
              Center(
                child: SizedBox(
                  height: 600,
                  width: 350,
                  child: CardSwiper(
                    controller: cardSwiperController,
                    cardsCount: filteredFilms.length,
                    cardBuilder: (BuildContext context, int index, __, ___) {
                      Film currentFilm = filteredFilms[index];

                      return InkWell(
                        onTap: () {
                          int filmId = currentFilm.id!;
                          context.push('/listFilms/$filmId/comments');
                        },
                        child: Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          crossAxisAlignment: CrossAxisAlignment.center,
                          children: [
                            Center(
                              child: SizedBox(
                                height: 550,
                                width: 350,
                                child: SingleChildScrollView(
                                  child: Column(
                                    mainAxisAlignment: MainAxisAlignment.center,
                                    crossAxisAlignment:
                                        CrossAxisAlignment.start,
                                    children: [
                                      FilmPoster(
                                          posterPath: currentFilm.posterPath),
                                      const SizedBox(height: 10),
                                      Center(
                                        child: Text(
                                          currentFilm.title,
                                          style: const TextStyle(
                                            fontSize: 30,
                                            fontWeight: FontWeight.bold,
                                          ),
                                          textAlign: TextAlign.center,
                                        ),
                                      ),
                                      const SizedBox(height: 5),
                                      FilmAttribute(
                                          attribName: "Overview: ",
                                          attribValue: currentFilm.overview),
                                      const SizedBox(height: 5),
                                      FilmAttribute(
                                          attribName: "Release Date: ",
                                          attribValue: currentFilm.releaseDate),
                                      const SizedBox(height: 5),
                                      FilmAttribute(
                                          attribName: "Actors: ",
                                          attribValue: currentFilm.actors),
                                      const SizedBox(height: 5),
                                      FilmAttribute(
                                          attribName: "Director: ",
                                          attribValue: currentFilm.director),
                                      const SizedBox(height: 5),
                                      FilmAttribute(
                                          attribName: "Duration: ",
                                          attribValue: currentFilm.duration),
                                      const SizedBox(height: 10),
                                      CommentsWidget(filmId: currentFilm.id!),
                                    ],
                                  ),
                                ),
                              ),
                            ),
                          ],
                        ),
                      );
                    },
                    onSwipe: (previousIndex, currentIndex, direction) {
                      debugPrint(
                          'Card swiped from $previousIndex to $currentIndex in direction ${direction.name}');

                      if (previousIndex == filteredFilms.length - 1) {
                        setState(() {
                          isRoundCompleted = true;
                        });

                        print('Round completed!');
                      }

                      if (direction.name == 'right' ||
                          direction.name == 'top') {
                        Provider.of<UserFilmsController>(context, listen: false)
                            .addUserFilm(widget.userId,
                                filteredFilms[previousIndex].id!, true);
                        setState(() {
                          feedbackMessage = 'Liked!';
                          feedbackColor =
                              const Color.fromARGB(255, 46, 125, 48);
                        });
                      } else if (direction.name == 'left' ||
                          direction.name == 'bottom') {
                        Provider.of<UserFilmsController>(context, listen: false)
                            .addUserFilm(widget.userId,
                                filteredFilms[previousIndex].id!, false);
                        setState(() {
                          feedbackMessage = 'Disliked!';
                          feedbackColor = Colors.red;
                        });
                      }

                      Future.delayed(const Duration(seconds: 1), () {
                        setState(() {
                          feedbackMessage = '';
                        });
                      });

                      return true;
                    },
                    numberOfCardsDisplayed: 1,
                    backCardOffset:
                        const Offset(-1000, 0),
                  ),
                ),
              ),
            ],
          ),
        if (isRoundCompleted || filteredFilms.isEmpty)
          Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Center(
                child: SizedBox(
                  width: 180,
                  height: 60,
                  child: ElevatedButton(
                    onPressed: () {
                      Provider.of<UserFilmsController>(context, listen: false)
                          .deleteAllUserFilms(widget.userId);

                      setState(() {
                        viewedFilmIds = [];
                        isRoundCompleted = false;
                      });
                    },
                    child: const Text(
                      'Swipe Again',
                      style: TextStyle(fontSize: 20),
                    ),
                  ),
                ),
              ),
              const SizedBox(height: 16),
              Center(
                child: SizedBox(
                  width: 180,
                  height: 60,
                  child: ElevatedButton(
                    child: const Text(
                      'View results',
                      style: TextStyle(fontSize: 20),
                    ),
                    onPressed: () {
                      context.push('/statsScreen');
                    },
                  ),
                ),
              ),
            ],
          ),
        Positioned(
          bottom: 16,
          left: 0,
          right: 0,
          child: Center(
            child: Text(
              feedbackMessage,
              style: TextStyle(
                fontSize: 20,
                fontWeight: FontWeight.bold,
                color: feedbackColor,
              ),
            ),
          ),
        ),
      ],
    );
  }

  @override
  void dispose() {
    cardSwiperController.dispose();
    super.dispose();
  }
}
