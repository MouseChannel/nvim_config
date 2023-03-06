vim.cmd('set expandtab')

local opt = vim.opt

opt.relativenumber = true
opt.number = true

opt.tabstop = 2
opt.shiftwidth = 2
--opt.expendtab = true
opt.autoindent = true

opt.wrap = false

opt.cursorline = true






opt.splitright = true
opt.splitbelow = true

opt.ignorecase = true
opt.smartcase = true

opt.termguicolors = true
opt.signcolumn = "yes"


vim.cmd[[colorscheme tokyonight-night]]




