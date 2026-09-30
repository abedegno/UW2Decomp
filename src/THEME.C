extern unsigned char CurrentPlayingMusicNo, NewThemeMusicToPlay;
void far ChangeThemeMusic(unsigned char theme)
{
    if (CurrentPlayingMusicNo != 6)
        NewThemeMusicToPlay = theme;
}
