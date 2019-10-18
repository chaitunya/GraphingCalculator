set colorcolumn=110
highlight ColorColumn ctermbg=darkgray
" nnoremap <F4> :make!<cr>
nnoremap <F4> :make!<cr>
nnoremap <F5> :!./GraphingCalculator<cr>
nnoremap <silent> <F6> :!ctags -R -f .tags .<cr>
autocmd VimEnter * NERDTree | wincmd p

set softtabstop=2
set tabstop=2
set expandtab
set shiftwidth=2

let g:ale_cpp_gcc_options = '-DQT_CORE_LIB -DQT_GUI_LIB -DQT_WIDGETS_LIB -I/home/chaitu/Documents/GraphingCalculator/GraphingCalculator_autogen/include -I/home/chaitu/Documents/GraphingCalculator/include -I/usr/local/include -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -isystem /usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui   -std=c++11 -g   -fPIC -std=gnu++11'

" " let g:syntastic_loc_list_height = 
" function! SyntasticCheckHook(errors)
"     if !empty(a:errors)
"         let g:syntastic_loc_list_height = min([len(a:errors) + 1, 10])
"     endif
" endfunction
" autocmd InsertLeave * SyntasticCheck

