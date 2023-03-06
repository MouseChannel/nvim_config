--[[
Author: mousechannel mochenghh@gmail.com
Date: 2023-03-05 11:13:33
LastEditors: mousechannel mochenghh@gmail.com
LastEditTime: 2023-03-05 11:14:03
FilePath: \nvim\lua\plugins\treesitter.lua
Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD
--]]
require'nvim-treesitter.configs'.setup {
    -- A list of parser names, or "all" (the five listed parsers should always be installed)
    ensure_installed = { "c", "lua", "vim", "help", "cpp" },
    highlight = {enable = true},
    indent = {enable = true},
    rainbow ={
        enable = true,
        extended_mode = true,
        max_file_lines = nil
    }

}  