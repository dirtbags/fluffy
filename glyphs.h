#pragma once

/* List of visually distinct glyphs.
 *
 * The goal here is visually distinct, 
 * not perfect fidelity with the Color Graphics Adapter from 1981,
 * although it certainly borrows heavily from CP437.
 * Also I refuse to give up the first 16, 
 * since I've been seeing them for 40 years.
 * 
 * It's ~~2021~~ 2024 and the state of monospace glyphs is *still* a mess.
 * Not that I blame anybody. 
 * The notion of monospacing needs to go away.
 * I'm going to need to figure out a smarter way to display 
 * grids of characters in a terminal.
 *
 * This renders as fixed width using:
 *
 * - Cascadia Code in Emacs on Windows 11
 * - Whatever I'm using in Chrome on Windows 11
 * 
 *  */
const char *fluffyglyphs[] = {
	"·", "☺", "☻", "♥", "♦", "♣", "♠", "•", "◘", "○", "◙", "♂", "♀", "♪", "♫", "☼",
	"►", "◄", "↕", "‼", "¶", "§", "‽", "↨", "↑", "↓", "→", "←", "∟", "↔", "▲", "▼",
	" ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?",
	"@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
	"P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_",
	"`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
	"p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~", "⌂",

	"Ç", "ü", "é", "â", "ä", "à", "å", "ç", "ê", "ë", "è", "ï", "î", "ì", "Ä", "Å",
	"É", "æ", "Æ", "ô", "ö", "ò", "û", "ù", "ÿ", "Ö", "Ü", "¢", "£", "¥", "₧", "ƒ",
	"á", "í", "ó", "ú", "ñ", "Ñ", "ª", "º", "¿", "⌐", "¬", "½", "¼", "¡", "«", "»",
	"░", "▒", "▓", "│", "┤", "╡", "╢", "╖", "╕", "╣", "║", "╗", "╝", "╜", "╛", "┐",
	"└", "┴", "┬", "├", "─", "┼", "╞", "╟", "╚", "╔", "╩", "╦", "╠", "═", "╬", "╧",
	"╨", "╤", "╥", "╙", "╘", "╒", "╓", "╫", "╪", "┘", "┌", "█", "▄", "▌", "▐", "▀",
	"α", "ß", "Γ", "π", "Σ", "σ", "µ", "τ", "Φ", "Θ", "Ω", "δ", "∞", "φ", "ε", "∩",
	"≡", "±", "≥", "≤", "⌠", "⌡", "÷", "≈", "°", "∞", "⊻", "√", "ⁿ", "²", "■", "¤",
};
