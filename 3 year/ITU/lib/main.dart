/// Authors:
/// Vadim Goncearenco (xgonce00@stud.fit.vutbr.cz)
///

import 'package:flutter/material.dart';
import 'package:itu_app/controller/comment_controller.dart';
import 'package:itu_app/controller/user_controller.dart';
import 'package:itu_app/controller/user_films_controller.dart';
import 'package:itu_app/model/db_helper.dart';
import 'package:itu_app/view/comments_screen.dart';
import 'package:itu_app/view/gallery_screen.dart';
import 'package:itu_app/view/films_list_screen.dart';
import 'package:itu_app/view/film_query_screen.dart';
import 'package:itu_app/view/home_screen.dart';

import 'dart:io' show Platform;

import 'package:flutter/foundation.dart' show kIsWeb;
import 'package:go_router/go_router.dart';
import 'package:itu_app/view/stats_screen.dart';
import 'package:provider/provider.dart';
import 'package:window_size/window_size.dart';

import 'package:itu_app/controller/film_controller.dart';

void main() {
  setupWindow();
  runApp(MultiProvider(providers: [
    ChangeNotifierProvider(create: (context) => FilmController()),
    ChangeNotifierProvider(create: (context) => CommentController()),
    ChangeNotifierProvider(create: (context) => UserFilmsController()),
    ChangeNotifierProvider(create: (context) => DatabaseHelper()),
    ChangeNotifierProvider(create: (context) => UserController()),
  ], child: const MyApp()));
}

const double windowWidth = 400;
const double windowHeight = 800;

void setupWindow() {
  if (!kIsWeb && (Platform.isWindows || Platform.isLinux || Platform.isMacOS)) {
    WidgetsFlutterBinding.ensureInitialized();
    setWindowTitle('Provider Demo');
    setWindowMinSize(const Size(windowWidth, windowHeight));
    setWindowMaxSize(const Size(windowWidth, windowHeight));
    getCurrentScreen().then((screen) {
      setWindowFrame(Rect.fromCenter(
        center: screen!.frame.center,
        width: windowWidth,
        height: windowHeight,
      ));
    });
  }
}

GoRouter router() {
  return GoRouter(
    initialLocation: '/homeScreen',
    routes: [
      GoRoute(
        path: '/homeScreen',
        builder: (context, state) => const HomeScreen(),
      ),
      GoRoute(
        path: '/statsScreen',
        builder: (context, state) => const StatsScreen(),
      ),
      GoRoute(
        path: '/filmGallery/:userId',
        builder: (context, state) {
          final userId = int.parse(state.pathParameters['userId']!);
          return GalleryScreen(userId: userId);
        },
      ),
      GoRoute(
        path: '/queryScreen',
        builder: (context, state) => const FilmQueryScreen(),
      ),
      GoRoute(
          path: '/listFilms',
          builder: (context, state) => const FilmsList(),
          routes: [
            GoRoute(
              path: ':filmId/comments',
              builder: (context, state) => CommentsScreen(
                  filmIndex: int.parse(state.pathParameters['filmId']!)),
            ),
          ]),
    ],
  );
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp.router(
      title: "Title",
      routerConfig: router(),
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        colorScheme: ColorScheme.fromSwatch().copyWith(
          primary: const Color.fromARGB(255, 68, 70, 115),
          secondary: const Color.fromARGB(255, 68, 70, 115),
        ),
        scaffoldBackgroundColor: const Color.fromARGB(255, 197, 202, 224),
        textTheme: const TextTheme(
            bodyMedium: TextStyle(
          color: Colors.black,
        )),
      ),
    );
  }
}
