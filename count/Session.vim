let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <F12> <F12>
imap <S-F3> <S-F3>
imap <C-F3> <C-F3>
imap <C-S-F3> <C-S-F3>
inoremap <Up> gk
inoremap <Down> gj
imap <F1> :se invpastea
map <NL> :cn
map  :cp
nmap  :up:make
map   :up|b #
vmap # y?\V=substitute(escape(@",'/\'),"\n","\\\\n","g")?
nnoremap ' `
vmap * y/\V=substitute(escape(@",'/\'),"\n","\\\\n","g")/
map ,e :e =expand("%:p:h") . "/" 
map ,s :sb 
map ,10 :b 10
map ,9 :b 9
map ,8 :b 8
map ,7 :b 7
map ,6 :b 6
map ,4 :b 4
map ,3 :b 3
map ,2 :b 2
map ,b :b 
map - _
vnoremap . :normal .
inoremap ¯ =CleverTab()
nmap K <Plug>ManPageView
vmap [% [%m'gv``
nmap <silent> \a? \WS:AlignCtrl mIp1P1lC ? : : : : :'a,.Align:'a,'z-1s/\(\s\+\)? /?\1/e\WE
nmap <silent> \a, \WS:'y,'zs/\(.\)\s\+/\1 /g'yjma'zk\jnr,:silent 'y,'zg/,/let @x=substitute(getline(line(".")),'^\(.\{-}\) \S\+\s*,.*$','silent s/,/;\\r\1 /g','')|@x\WE
nmap <silent> \abox \WS:let b:alignmaps_iws=substitute(getline("'a"),'^\(\s*\).*$','\1','e'):'a,'z-1s/^\s\+//e:'a,'z-1s/^.*$/@&@/:AlignCtrl m=p01P0w @:'a,.Align:'a,'z-1s/@/ * /:'a,'z-1s/@$/*/'aYP:s/./*/g0r/'zkYp:s/./*/g0r A/:exe "'a-1,'z-1s/^/".b:alignmaps_iws."/e"\WE
nmap <silent> \acom \WS:'a,.s/\/[*/]/@&@/e:'a,.s/\*\//@&/e'zk\tW@:'y,'zs/^\(\s*\) @/\1/e:'y,'zs/ @//eg\WE
nmap <silent> \ascom \WS:'a,.s/\/[*/]/@&@/e:'a,.s/\*\//@&/e:silent! 'a,.g/^\s*@\/[*/]/s/@//ge:AlignCtrl v ^\s*\/[*/]:AlignCtrl g \/[*/]'zk\tW@:'y,'zs/^\(\s*\) @/\1/e:'y,'zs/ @//eg\WE
nmap <silent> \adec \WS:'a,'zs/\([^ \t/(]\)\([*&]\)/\1 \2/e:'y,'zv/^\//s/\([^ \t]\)\s\+/\1 /ge:'y,'zv/^\s*[*/]/s/\([^/][*&]\)\s\+/\1/ge:'y,'zv/^\s*[*/]/s/^\(\s*\%(\h\w*\s\+\%([a-zA-Z_*(&]\)\@=\)\+\)\([*(&]*\)\s*\([a-zA-Z0-9_()]\+\)\s*\(\(\[.\{-}]\)*\)\s*\(=\)\=\s*\(.\{-}\)\=\s*;/\1@\2#@\3\4@\6@\7;@/e:'y,'zv/^\s*[*/]/s/\*\/\s*$/@*\//e:'y,'zv/^\s*[*/]/s/^\s\+\*/@@@@@* /e:'y,'zv/^\s*[*/]/s/^@@@@@\*\(.*[^*/]\)$/&@*/e'yjma'zk:AlignCtrl v ^\s*[*/#]\t@:'y,'zv/^\s*[*/]/s/@ //ge:'y,'zv/^\s*[*/]/s/\(\s*\);/;\1/e:'y,'zv/^#/s/# //e:'y,'zv/^\s\+[*/#]/s/\([^/*]\)\(\*\+\)\( \+\)/\1\3\2/e:'y,'zv/^\s\+[*/#]/s/\((\+\)\( \+\)\*/\2\1*/e:'y,'zv/^\s\+[*/#]/s/^\(\s\+\) \*/\1*/e:'y,'zv/^\s\+[*/#]/s/[ \t@]*$//e:'y,'zs/^[*]/ */e\WE
nmap <silent> \adef \WS:AlignPush:AlignCtrl v ^\s*\(\/\*\|\/\/\):'a,.v/^\s*\(\/\*\|\/\/\)/s/^\(\s*\)#\(\s\)*define\s*\(\I[a-zA-Z_0-9(),]*\)\s*\(.\{-}\)\($\|\/\*\)/#\1\2define @\3@\4@\5/e:'a,.v/^\s*\(\/\*\|\/\/\)/s/\($\|\*\/\)/@&/e'zk\t@'yjma'zk:'a,.v/^\s*\(\/\*\|\/\/\)/s/ @//g\WE
nmap <silent> \anum \WS:'a,'zs/\(\d\)\s\+\(-\=[.,]\=\d\)/\1@\2/ge:AlignCtrl mp0P0:'a,'zAlign [.,@]:'a,'zs/\([-0-9.,]*\)\(\s*\)\([.,]\)/\2\1\3/g:'a,'zs/@/ /ge\WE
nmap <silent> \Htd \WS:'y,'zs%<[tT][rR]><[tT][dD][^>]\{-}>\|</[tT][dD]><[tT][dD][^>]\{-}>\|</[tT][dD]></[tT][rR]>%@&@%g'yjma'zk:AlignCtrl m=Ilp1P0 @:'a,.Align:'y,'zs/ @/@/:'y,'zs/@ <[tT][rR]>/<[tT][rR]>/ge:'y,'zs/@//ge\WE
nmap <silent> \T| \WS:AlignCtrl mIp0P0=r |:'a,.Align\WE
nmap <silent> \T, \WS:AlignCtrl mIp0P1=r ,:'a,.Align\WE
nmap <silent> \Ts, \WS:AlignCtrl mIp0P1=r ,:'a,.Align:'a,.s/\(\s*\),/,\1/ge\WE
nmap <silent> \T: \WS:AlignCtrl mIp1P1=r ::'a,.Align\WE
nmap <silent> \T< \WS:AlignCtrl mIp0P0=r <:'a,.Align\WE
nmap <silent> \T= \WS:'a,'z-1s/\s\+\([*/+\-%|&\~^]\==\)/ \1/e:'a,'z-1s@ \+\([*/+\-%|&\~^]\)=@\1=@ge:'a,'z-1s/; */;@/e:'a,'z-1s/==/``/ge:'a,'z-1s/!=/!`/ge:AlignCtrl mIp1P1=r = @:AlignCtrl g =:'a,'z-1Align:'a,'z-1s/; *@/;/e:'a,'z-1s/; *$/;/e:'a,'z-1s@\([*/+\-%|&\~^]\)\( \+\)=@\2\1=@ge:'a,'z-1s/\( \+\);/;\1/ge:'a,'z-1s/`/=/ge\WE\acom
nmap <silent> \T@ \WS:AlignCtrl mIp0P0=r @:'a,.Align\WE
nmap <silent> \Tsp \WS:'a,.s/^\(\s*\)\(.*\)/\=submatch(1).escape(substitute(submatch(2),'\s\+','@','g'),'\')/:AlignCtrl mI=r @:'a,.Align:'y+1,'z-1s/@/ /g\WE
nmap <silent> \t| \WS:AlignCtrl mIp0P0=l |:'a,.Align\WE
nmap <silent> \t, \WS:AlignCtrl mIp0P1=l ,:'a,.Align\WE
nmap <silent> \ts, \WS:AlignCtrl mIp0P0=l ,:'a,.Align:'a,.s/\(\s*\),/,\1/ge\WE
nmap <silent> \t: \WS:AlignCtrl mIp1P1=l ::'a,.Align\WE
nmap <silent> \t; \WS:AlignCtrl mIp0P0=l ;:'a,.Align:.,'zs/ \( *\);/;\1/ge\WE
nmap <silent> \t< \WS:AlignCtrl mIp0P0=l <:'a,.Align\WE
nmap <silent> \t= \WS:'a,'zs/\s\+\([*/+\-%|&\~^]\==\)/ \1/e:'a,'zs@ \+\([*/+\-%|&\~^]\)=@\1=@ge:'a,'zs/==/``/ge:'a,'zs/!=/!`/ge'zk:AlignCtrl mIp1P1=l =:AlignCtrl g =:'a,'z-1Align:'a,'z-1s@\([*/+\-%|&\~^!=]\)\( \+\)=@\2\1=@ge:'a,'z-1s/\( \+\);/;\1/ge:'a,'z-1v/^\s*\/[*/]/s/\/[*/]/@&@/e:'a,'z-1v/^\s*\/[*/]/s/\*\//@&/e'zk\t@:'y,'zs/^\(\s*\) @/\1/e:'a,'z-1s/`/=/ge:'y,'zs/ @//eg\WE
nmap <silent> \t? \WS:AlignCtrl mIp0P0=l ?:'a,.Align:.,'zs/ \( *\);/;\1/ge\WE
nmap <silent> \tab \WS:'a,.s/^\(\t*\)\(.*\)/\=submatch(1).escape(substitute(submatch(2),'\t','@','g'),'\')/:AlignCtrl mI=l @:'a,.Align:'y+1,'z-1s/@/ /g\WE
nmap <silent> \tsp \WS:'a,.s/^\(\s*\)\(.*\)/\=submatch(1).escape(substitute(submatch(2),'\s\+','@','g'),'\')/:AlignCtrl mI=l @:'a,.Align:'y+1,'z-1s/@/ /g\WE
nmap <silent> \tsq \WS:'a,.ReplaceQuotedSpaces:'a,.s/^\(\s*\)\(.*\)/\=submatch(1).substitute(submatch(2),'\s\+','@','g')/:AlignCtrl mIp0P0=l @:'a,.Align:'y+1,'z-1s/[%@]/ /g\WE
nmap <silent> \tt \WS:AlignCtrl mIp1P1=l \\\@<!& \\\\:'a,.Align\WE
nmap <silent> \t@ :AlignCtrl mIp1P1=l @:'a,.Align
nmap \sh <Plug>DBHistory
nmap \slv <Plug>DBListView
nmap \slp <Plug>DBListProcedure
nmap \slt <Plug>DBListTable
vmap <silent> \slc :exec 'DBListColumn '.DB_getVisualBlock()
nmap \slc <Plug>DBListColumn
nmap \sbp <Plug>DBPromptForBufferParameters
nmap \sdpa <Plug>DBDescribeProcedureAskName
vmap <silent> \sdp :exec 'DBDescribeProcedure '.DB_getVisualBlock()
nmap \sdp <Plug>DBDescribeProcedure
nmap \sdta <Plug>DBDescribeTableAskName
vmap <silent> \sdt :exec 'DBDescribeTable '.DB_getVisualBlock()
nmap \sdt <Plug>DBDescribeTable
nmap \sta <Plug>DBSelectFromTableAskName
nmap \stw <Plug>DBSelectFromTableWithWhere
vmap <silent> \st :exec 'DBSelectFromTable '.DB_getVisualBlock()
nmap \st <Plug>DBSelectFromTable
nmap <silent> \sel :.,.DBExecRangeSQL
nmap <silent> \sea :1,$DBExecRangeSQL
nmap \se <Plug>DBExecSQLUnderCursor
vmap \se <Plug>DBExecVisualSQL
nmap \cwr <Plug>CVSWatchRemove
nmap \cwf <Plug>CVSWatchOff
nmap \cwn <Plug>CVSWatchOn
nmap \cwa <Plug>CVSWatchAdd
nmap \cwv <Plug>CVSWatchers
nmap \cv <Plug>CVSVimDiff
nmap \cu <Plug>CVSUpdate
nmap \ct <Plug>CVSUnedit
nmap \cs <Plug>CVSStatus
nmap \cr <Plug>CVSReview
nmap \cq <Plug>CVSRevert
nmap \cl <Plug>CVSLog
nmap \cg <Plug>CVSGotoOriginal
nmap \ci <Plug>CVSEditors
nmap \ce <Plug>CVSEdit
nmap \cd <Plug>CVSDiff
nmap \cc <Plug>CVSCommit
nmap \cG <Plug>CVSClearAndGotoOriginal
nmap \cn <Plug>CVSAnnotate
nmap \ca <Plug>CVSAdd
map <silent> \bv :VSBufExplorer
map <silent> \bs :SBufExplorer
map <silent> \be :BufExplorer
nmap \scp <Plug>SQLUCreateProcedure
nmap \scdt <Plug>SQLUGetColumnDataType
nmap \scd <Plug>SQLUGetColumnDef
nmap \scl <Plug>SQLUCreateColumnList
vmap \sf <Plug>SQLUFormatter
nmap \sf <Plug>SQLUFormatter
vmap <silent> \Htd :<BS><BS><BS>ma'>\Htd
vmap <silent> \tt :<BS><BS><BS>ma'>\tt
vmap <silent> \tp@ :<BS><BS><BS>ma'>\tp@
vmap <silent> \tsq :<BS><BS><BS>ma'>\tsq
vmap <silent> \tsp :<BS><BS><BS>ma'>\tsp
vmap <silent> \tab :<BS><BS><BS>ma'>\tab
vmap <silent> \t@ :<BS><BS><BS>ma'>\t@
vmap <silent> \t? :<BS><BS><BS>ma'>\t?
vmap <silent> \t= :<BS><BS><BS>ma'>\t=
vmap <silent> \t< :<BS><BS><BS>ma'>\t<
vmap <silent> \t; :<BS><BS><BS>ma'>\t;
vmap <silent> \t: :<BS><BS><BS>ma'>\t:
vmap <silent> \ts, :<BS><BS><BS>ma'>\ts,
vmap <silent> \t, :<BS><BS><BS>ma'>\t,
vmap <silent> \t| :<BS><BS><BS>ma'>\t|
vmap <silent> \anum :B s/\(\d\)\s\+\(-\=[.,]\=\d\)/\1@\2/ge:AlignCtrl mp0P0gv:Align [.,@]:'<,'>s/\([-0-9.,]*\)\(\s\+\)\([.,]\)/\2\1\3/ge:'<,'>s/@/ /ge
vmap <silent> \afnc :<BS><BS><BS>ma'>\afnc
vmap <silent> \adef :<BS><BS><BS>ma'>\adef
vmap <silent> \adec :<BS><BS><BS>ma'>\adec
vmap <silent> \ascom :<BS><BS><BS>ma'>\ascom
vmap <silent> \acom :<BS><BS><BS>ma'>\acom
vmap <silent> \abox :<BS><BS><BS>ma'>\abox
vmap <silent> \a, :<BS><BS><BS>ma'>\a,
vmap <silent> \a? :<BS><BS><BS>ma'>\a?
vmap <silent> \Tsp :<BS><BS><BS>ma'>\Tsp
vmap <silent> \T@ :<BS><BS><BS>ma'>\T@
vmap <silent> \T= :<BS><BS><BS>ma'>\T=
vmap <silent> \T< :<BS><BS><BS>ma'>\T<
vmap <silent> \T: :<BS><BS><BS>ma'>\T:
vmap <silent> \Ts, :<BS><BS><BS>ma'>\Ts,
vmap <silent> \T, :<BS><BS><BS>ma'>\T,
vmap <silent> \T| :<BS><BS><BS>ma'>\T|
map <silent> \tW@ :AlignCtrl mWp1P1=l @:'a,.Align
omap <silent> \t@ :AlignCtrl mIp1P1=l @:'a,.Align
map <silent> \t~ \WS:AlignCtrl mIp0P0=l ~:'a,.Align:'y,'zs/ \( *\);/;\1/ge\WE
omap <silent> \tt \WS:AlignCtrl mIp1P1=l \\\@<!& \\\\:'a,.Align\WE
omap <silent> \tsq \WS:'a,.ReplaceQuotedSpaces:'a,.s/^\(\s*\)\(.*\)/\=submatch(1).substitute(submatch(2),'\s\+','@','g')/:AlignCtrl mIp0P0=l @:'a,.Align:'y+1,'z-1s/[%@]/ /g\WE
omap <silent> \tsp \WS:'a,.s/^\(\s*\)\(.*\)/\=submatch(1).escape(substitute(submatch(2),'\s\+','@','g'),'\')/:AlignCtrl mI=l @:'a,.Align:'y+1,'z-1s/@/ /g\WE
omap <silent> \tab \WS:'a,.s/^\(\t*\)\(.*\)/\=submatch(1).escape(substitute(submatch(2),'\t','@','g'),'\')/:AlignCtrl mI=l @:'a,.Align:'y+1,'z-1s/@/ /g\WE
omap <silent> \t? \WS:AlignCtrl mIp0P0=l ?:'a,.Align:.,'zs/ \( *\);/;\1/ge\WE
omap <silent> \t= \WS:'a,'zs/\s\+\([*/+\-%|&\~^]\==\)/ \1/e:'a,'zs@ \+\([*/+\-%|&\~^]\)=@\1=@ge:'a,'zs/==/``/ge:'a,'zs/!=/!`/ge'zk:AlignCtrl mIp1P1=l =:AlignCtrl g =:'a,'z-1Align:'a,'z-1s@\([*/+\-%|&\~^!=]\)\( \+\)=@\2\1=@ge:'a,'z-1s/\( \+\);/;\1/ge:'a,'z-1v/^\s*\/[*/]/s/\/[*/]/@&@/e:'a,'z-1v/^\s*\/[*/]/s/\*\//@&/e'zk\t@:'y,'zs/^\(\s*\) @/\1/e:'a,'z-1s/`/=/ge:'y,'zs/ @//eg\WE
omap <silent> \t< \WS:AlignCtrl mIp0P0=l <:'a,.Align\WE
omap <silent> \t; \WS:AlignCtrl mIp0P0=l ;:'a,.Align:.,'zs/ \( *\);/;\1/ge\WE
omap <silent> \t: \WS:AlignCtrl mIp1P1=l ::'a,.Align\WE
omap <silent> \ts, \WS:AlignCtrl mIp0P0=l ,:'a,.Align:'a,.s/\(\s*\),/,\1/ge\WE
omap <silent> \t, \WS:AlignCtrl mIp0P1=l ,:'a,.Align\WE
map <silent> \t# \WS:AlignCtrl mIp0P0=l #:'a,.Align\WE
omap <silent> \t| \WS:AlignCtrl mIp0P0=l |:'a,.Align\WE
map <silent> \T~ \WS:AlignCtrl mIp0P0=r ~:'a,.Align:'y,'zs/ \( *\);/;\1/ge\WE
omap <silent> \Tsp \WS:'a,.s/^\(\s*\)\(.*\)/\=submatch(1).escape(substitute(submatch(2),'\s\+','@','g'),'\')/:AlignCtrl mI=r @:'a,.Align:'y+1,'z-1s/@/ /g\WE
map <silent> \Tab \WS:'a,.s/^\(\t*\)\(.*\)/\=submatch(1).escape(substitute(submatch(2),'\t','@','g'),'\')/:AlignCtrl mI=r @:'a,.Align:'y+1,'z-1s/@/ /g\WE
omap <silent> \T@ \WS:AlignCtrl mIp0P0=r @:'a,.Align\WE
map <silent> \T? \WS:AlignCtrl mIp0P0=r ?:'a,.Align:'y,'zs/ \( *\);/;\1/ge\WE
omap <silent> \T= \WS:'a,'z-1s/\s\+\([*/+\-%|&\~^]\==\)/ \1/e:'a,'z-1s@ \+\([*/+\-%|&\~^]\)=@\1=@ge:'a,'z-1s/; */;@/e:'a,'z-1s/==/``/ge:'a,'z-1s/!=/!`/ge:AlignCtrl mIp1P1=r = @:AlignCtrl g =:'a,'z-1Align:'a,'z-1s/; *@/;/e:'a,'z-1s/; *$/;/e:'a,'z-1s@\([*/+\-%|&\~^]\)\( \+\)=@\2\1=@ge:'a,'z-1s/\( \+\);/;\1/ge:'a,'z-1s/`/=/ge\WE\acom
omap <silent> \T< \WS:AlignCtrl mIp0P0=r <:'a,.Align\WE
map <silent> \T; \WS:AlignCtrl mIp0P0=r ;:'a,.Align\WE
omap <silent> \T: \WS:AlignCtrl mIp1P1=r ::'a,.Align\WE
omap <silent> \Ts, \WS:AlignCtrl mIp0P1=r ,:'a,.Align:'a,.s/\(\s*\),/,\1/ge\WE
omap <silent> \T, \WS:AlignCtrl mIp0P1=r ,:'a,.Align\WE
map <silent> \T# \WS:AlignCtrl mIp0P0=r #:'a,.Align\WE
omap <silent> \T| \WS:AlignCtrl mIp0P0=r |:'a,.Align\WE
omap <silent> \Htd \WS:'y,'zs%<[tT][rR]><[tT][dD][^>]\{-}>\|</[tT][dD]><[tT][dD][^>]\{-}>\|</[tT][dD]></[tT][rR]>%@&@%g'yjma'zk:AlignCtrl m=Ilp1P0 @:'a,.Align:'y,'zs/ @/@/:'y,'zs/@ <[tT][rR]>/<[tT][rR]>/ge:'y,'zs/@//ge\WE
omap <silent> \anum \WS:'a,'zs/\(\d\)\s\+\(-\=[.,]\=\d\)/\1@\2/ge:AlignCtrl mp0P0:'a,'zAlign [.,@]:'a,'zs/\([-0-9.,]*\)\(\s*\)\([.,]\)/\2\1\3/g:'a,'zs/@/ /ge\WE
omap <silent> \adef \WS:AlignPush:AlignCtrl v ^\s*\(\/\*\|\/\/\):'a,.v/^\s*\(\/\*\|\/\/\)/s/^\(\s*\)#\(\s\)*define\s*\(\I[a-zA-Z_0-9(),]*\)\s*\(.\{-}\)\($\|\/\*\)/#\1\2define @\3@\4@\5/e:'a,.v/^\s*\(\/\*\|\/\/\)/s/\($\|\*\/\)/@&/e'zk\t@'yjma'zk:'a,.v/^\s*\(\/\*\|\/\/\)/s/ @//g\WE
omap <silent> \adec \WS:'a,'zs/\([^ \t/(]\)\([*&]\)/\1 \2/e:'y,'zv/^\//s/\([^ \t]\)\s\+/\1 /ge:'y,'zv/^\s*[*/]/s/\([^/][*&]\)\s\+/\1/ge:'y,'zv/^\s*[*/]/s/^\(\s*\%(\h\w*\s\+\%([a-zA-Z_*(&]\)\@=\)\+\)\([*(&]*\)\s*\([a-zA-Z0-9_()]\+\)\s*\(\(\[.\{-}]\)*\)\s*\(=\)\=\s*\(.\{-}\)\=\s*;/\1@\2#@\3\4@\6@\7;@/e:'y,'zv/^\s*[*/]/s/\*\/\s*$/@*\//e:'y,'zv/^\s*[*/]/s/^\s\+\*/@@@@@* /e:'y,'zv/^\s*[*/]/s/^@@@@@\*\(.*[^*/]\)$/&@*/e'yjma'zk:AlignCtrl v ^\s*[*/#]\t@:'y,'zv/^\s*[*/]/s/@ //ge:'y,'zv/^\s*[*/]/s/\(\s*\);/;\1/e:'y,'zv/^#/s/# //e:'y,'zv/^\s\+[*/#]/s/\([^/*]\)\(\*\+\)\( \+\)/\1\3\2/e:'y,'zv/^\s\+[*/#]/s/\((\+\)\( \+\)\*/\2\1*/e:'y,'zv/^\s\+[*/#]/s/^\(\s\+\) \*/\1*/e:'y,'zv/^\s\+[*/#]/s/[ \t@]*$//e:'y,'zs/^[*]/ */e\WE
omap <silent> \ascom \WS:'a,.s/\/[*/]/@&@/e:'a,.s/\*\//@&/e:silent! 'a,.g/^\s*@\/[*/]/s/@//ge:AlignCtrl v ^\s*\/[*/]:AlignCtrl g \/[*/]'zk\tW@:'y,'zs/^\(\s*\) @/\1/e:'y,'zs/ @//eg\WE
omap <silent> \acom \WS:'a,.s/\/[*/]/@&@/e:'a,.s/\*\//@&/e'zk\tW@:'y,'zs/^\(\s*\) @/\1/e:'y,'zs/ @//eg\WE
omap <silent> \abox \WS:let b:alignmaps_iws=substitute(getline("'a"),'^\(\s*\).*$','\1','e'):'a,'z-1s/^\s\+//e:'a,'z-1s/^.*$/@&@/:AlignCtrl m=p01P0w @:'a,.Align:'a,'z-1s/@/ * /:'a,'z-1s/@$/*/'aYP:s/./*/g0r/'zkYp:s/./*/g0r A/:exe "'a-1,'z-1s/^/".b:alignmaps_iws."/e"\WE
map <silent> \a< \WS:AlignCtrl mIp1P1=l << >>:'a,.Align\WE
omap <silent> \a, \WS:'y,'zs/\(.\)\s\+/\1 /g'yjma'zk\jnr,:silent 'y,'zg/,/let @x=substitute(getline(line(".")),'^\(.\{-}\) \S\+\s*,.*$','silent s/,/;\\r\1 /g','')|@x\WE
omap <silent> \a? \WS:AlignCtrl mIp1P1lC ? : : : : :'a,.Align:'a,'z-1s/\(\s\+\)? /?\1/e\WE
nmap \WE <Plug>AlignMapsWrapperEnd
nmap \WS <Plug>AlignMapsWrapperStart
map <silent> \z :set foldmethod=expr foldlevel=0 foldcolumn=2 foldexpr=\(\g\e\t\l\i\n\e\(\v\:\l\n\u\m\)\=\~\@\/\)\?\0\:\(\g\e\t\l\i\n\e\(\v\:\l\n\u\m\-\1\)\=\~\@\/\)\\|\\|\(\g\e\t\l\i\n\e\(\v\:\l\n\u\m\+\1\)\=\~\@\/\)\?\1\:\2
vmap ]% ]%m'gv``
vnoremap ` :normal @a
nnoremap ` @a
nmap gx <Plug>NetrwBrowseX
vnoremap j gj
nnoremap j gj
vnoremap k gk
nnoremap k gk
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
nnoremap <silent> <Plug>CVSWatchRemove :CVSWatchRemove
nnoremap <silent> <Plug>CVSWatchOff :CVSWatchOff
nnoremap <silent> <Plug>CVSWatchOn :CVSWatchOn
nnoremap <silent> <Plug>CVSWatchAdd :CVSWatchAdd
nnoremap <silent> <Plug>CVSWatchers :CVSWatchers
nnoremap <silent> <Plug>CVSVimDiff :CVSVimDiff
nnoremap <silent> <Plug>CVSUpdate :CVSUpdate
nnoremap <silent> <Plug>CVSUnedit :CVSUnedit
nnoremap <silent> <Plug>CVSStatus :CVSStatus
nnoremap <silent> <Plug>CVSReview :CVSReview
nnoremap <silent> <Plug>CVSRevert :CVSRevert
nnoremap <silent> <Plug>CVSLog :CVSLog
nnoremap <silent> <Plug>CVSClearAndGotoOriginal :CVSGotoOriginal!
nnoremap <silent> <Plug>CVSGotoOriginal :CVSGotoOriginal
nnoremap <silent> <Plug>CVSEditors :CVSEditors
nnoremap <silent> <Plug>CVSEdit :CVSEdit
nnoremap <silent> <Plug>CVSDiff :CVSDiff
nnoremap <silent> <Plug>CVSCommit :CVSCommit
nnoremap <silent> <Plug>CVSAnnotate :CVSAnnotate
nnoremap <silent> <Plug>CVSAdd :CVSAdd
map <F7> :mks! | wqa 
map <F12> :silent nohl
nmap <F5> Ofprintf(stderr, "*** HERE ***\n");
map <S-F9> :setlocal foldexpr=getline(v:lnum)=~\"^[\ ]*[*#/]\" foldmethod=expr
map <silent> <F9> :set foldexpr=getline(v:lnum)=~'^\\s*$'&&getline(v:lnum+1)=~'\\S'?'<1':1
nnoremap <silent> <S-F8> :TlistSync
nnoremap <silent> <F8> :Tlist
map <F11> :call MouseModeSwitch()
map <kMinus> l|
map <kPlus> h|
map <F6> :!ctags --langmap='PHP:.inc.php.php3' -R 
map <S-F3> :call BrowserPreview()
map <C-F3> :call BrowserPreviewIE()
map <C-S-F3> :call BrowserPreviewOpera()
vnoremap <Up> gk
vnoremap <Down> gj
nnoremap <Up> gk
nnoremap <Down> gj
noremap <S-Insert> :w:! xterm -bg ivory -fn 10x20 -e ispell % :e % 
map <F1> :se invpaste paste?
imap  gUawea
inoremap 	 =CleverTab()
let &cpo=s:cpo_save
unlet s:cpo_save
set keymap=russian-yawerty
set autoindent
set backspace=indent,eol,start
set backup
set backupdir=~/.vimtmp,/tmp,/winnt/Temp,/windows/Temp
set cmdheight=2
set complete=.,w,b,u,t,i,k
set dictionary=~/.vim/dict/c
set directory=~/.vimtmp,/tmp,/winnt/Temp,/windows/Temp,.
set exrc
set fileencodings=utf-8,cp1251
set fileformats=unix,dos,mac
set nofsync
set guiheadroom=0
set helplang=en
set hidden
set history=50
set hlsearch
set ignorecase
set iminsert=0
set imsearch=0
set incsearch
set langmenu=none
set laststatus=2
set mouse=a
set pastetoggle=<F10>
set path=.,/usr/local/include,/usr/include,/usr/src/linux/include,,
set printoptions=paper:letter
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set sessionoptions=blank,buffers,curdir,folds,help,options,tabpages,winsize,unix,slash
set showcmd
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set swapsync=
set winminheight=0
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/my/works/compression/count_chars
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +1 count_entropy.c
args count_entropy.c
edit count_entropy.c
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
lnoremap <buffer> # ё
lnoremap <buffer> $ Ё
lnoremap <buffer> % ъ
lnoremap <buffer> + Ч
lnoremap <buffer> = ч
lnoremap <buffer> A А
lnoremap <buffer> B Б
lnoremap <buffer> C Ц
lnoremap <buffer> D Д
lnoremap <buffer> E Е
lnoremap <buffer> F Ф
lnoremap <buffer> G Г
lnoremap <buffer> H Х
lnoremap <buffer> I И
lnoremap <buffer> J Й
lnoremap <buffer> K К
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> L Л
lnoremap <buffer> M М
lnoremap <buffer> N Н
lnoremap <buffer> O О
lnoremap <buffer> P П
lnoremap <buffer> Q Я
lnoremap <buffer> R Р
lnoremap <buffer> S С
lnoremap <buffer> T Т
lnoremap <buffer> U У
lnoremap <buffer> V Ж
lnoremap <buffer> W В
lnoremap <buffer> X Ь
lnoremap <buffer> Y Ы
lnoremap <buffer> Z З
lnoremap <buffer> [ ш
lnoremap <buffer> \ э
lnoremap <buffer> ] щ
lnoremap <buffer> ^ Ъ
lnoremap <buffer> ` ю
lnoremap <buffer> a а
lnoremap <buffer> b б
lnoremap <buffer> c ц
lnoremap <buffer> d д
lnoremap <buffer> e е
lnoremap <buffer> f ф
lnoremap <buffer> g г
lnoremap <buffer> h х
lnoremap <buffer> i и
lnoremap <buffer> j й
lnoremap <buffer> k к
lnoremap <buffer> l л
lnoremap <buffer> m м
lnoremap <buffer> n н
lnoremap <buffer> o о
lnoremap <buffer> p п
lnoremap <buffer> q я
lnoremap <buffer> r р
lnoremap <buffer> s с
lnoremap <buffer> t т
lnoremap <buffer> u у
lnoremap <buffer> v ж
lnoremap <buffer> w в
lnoremap <buffer> x ь
lnoremap <buffer> y ы
lnoremap <buffer> z з
lnoremap <buffer> { Ш
lnoremap <buffer> | Э
lnoremap <buffer> } Щ
lnoremap <buffer> ~ Ю
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=russian-yawerty
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i,k
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal foldcolumn=0
setlocal foldenable
set foldexpr=getline(v:lnum)[0]==\"#\"
setlocal foldexpr=getline(v:lnum)[0]==\"#\"
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=expr
setlocal foldmethod=expr
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
setlocal nonumber
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 46 - ((45 * winheight(0) + 20) / 40)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
46
normal! 05|
tabnext 1
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
