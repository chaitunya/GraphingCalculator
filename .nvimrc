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
